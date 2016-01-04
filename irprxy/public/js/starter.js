$(document).ready(function(){
	function __convert (str){
		"{" + (str.split(",").map(function(elm){
			return "{" + [elm.substr(1), "L" == elm.substr(0,1) ? "LOW": "HIGH"].join(",") + "}";
		}).join(",")) + "}";
	}
	function __trim(str){
		return str.replace(/^\s+/,"").replace(/\s+$/, "");
	}

	var socket = io.connect("/");
	socket.on("connect", function(evt){
		$("#submit").click(function(){
			var txt = $("#input").val();
			if(txt){
				socket.json.send({"data":txt});
				$("#input").val("");
			}
		});
		$("#clean").click(function(){
			$("#output").val("");
		});
		$("#convert").click(function(){
			$("#cvoutput").val("");
			var txt = $("#output").val();
			if(txt){
				var lns = txt.split("\n");
				$("#cvoutput").val(__convert(__trim(lns[0])));
			}
		});
		var sw01 = null;
		var sw02 = null;
		$("#recandgen").click(function(){
			var next = $(".sw").not(".wait,.done")[0];
			if(next){
				next.addClass("wait");
				$("#output").val("");
				socket.json.send({"data":"R"});
			}
		})
	}).on("message", function(evt){
		if(evt.data){
			var txt = $("#output").val([$("#output").val(), evt.data].join(""));
			var wts = $(".sw.wait");
			if(wts.length){
				$(wts[0]).val("");
				var lns = txt.split("\n");
				$(wts[0]).val(__convert(__trim(lns[0])));
			}
		}
	});
	
});