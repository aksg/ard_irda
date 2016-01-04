var express = require('express');
var path = require('path');
var favicon = require('static-favicon');
var logger = require('morgan');
var cookieParser = require('cookie-parser');
var bodyParser = require('body-parser');

var routes = require('./routes/index');
var users = require('./routes/users');

var fs = require('fs');

var app = express();
/*
var ssloptions = {
	key: fs.readFileSync('./key/localhost.key'),
	cert:fs.readFileSync('./key/localhost.crt'),
	requestCert: false,
	rejectUnauthorized: false
};*/

var inotemplate = fs.readFileSync("./public/emitter.template.txt");

// view engine setup
app.set('views', path.join(__dirname, 'views'));
app.set('view engine', 'jade');

app.use(favicon());
app.use(logger('dev'));
app.use(bodyParser.json());
app.use(bodyParser.urlencoded());
app.use(cookieParser());
app.use(express.static(path.join(__dirname, 'public')));

app.use('/', routes);
app.use('/users', users);

/// catch 404 and forward to error handler
app.use(function(req, res, next) {
    var err = new Error('Not Found');
    err.status = 404;
    next(err);
});

/// error handlers

// development error handler
// will print stacktrace
if (app.get('env') === 'development') {
    app.use(function(err, req, res, next) {
        res.status(err.status || 500);
        res.render('error', {
            message: err.message,
            error: err
        });
    });
}

// production error handler
// no stacktraces leaked to user
app.use(function(err, req, res, next) {
    res.status(err.status || 500);
    res.render('error', {
        message: err.message,
        error: {}
    });
});

var log4js = require("log4js");
log4js.configure(process.env.LOGCONFIG || "log4js.config.nofile.json");
var msglogger = log4js.getLogger("msg");

var serialport = require("serialport");
/*serialport.list(function(err, ports){
	ports.forEach(function(port){
		console.log(port.comName);
		console.log(port.pnpId);
		console.log(port.manufacturer);
	});
});*/
var sp = new serialport.SerialPort(
	process.env.SRPX_PORT || "COM22",{
	baudRate: 1 * (process.env.SRPX_BAUD || 57600),//115200,//
	dataBits: 8,
	parity: "none",
	stopBits: 1,
	flowControl: false,
	parser: serialport.parsers.readline("\n")
});

sp.on("open", function(){
	setTimeout(function(){
		var port = 9876;
		module.exports = app;
		var io = require("socket.io").listen(
//			require('https').createServer(ssloptions, app).listen(port)
			app.listen(port, "localhost")
		);
		msglogger.info('App started on port ' + port);

		io.sockets.on("connection", function(socket){
			socket.on("message", function(message){
				if(message.data){
					var str = message.data;
					for(var i = 0; i < str.length; i+=100){
						(function(itr){setTimeout(function(){
							var prt = str.substr(itr,100);
							console.log(prt);
							sp.write(prt);
						},itr);})(i)
					}
				}
			});
		});
		sp.on("data", function(data){
			var buffer = new Buffer(data, "utf8");
			io.sockets.json.emit("message", {"data":""+buffer});
			console.log("" + buffer);
		});
	},800);
});
