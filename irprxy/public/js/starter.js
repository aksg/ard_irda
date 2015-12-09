$(document).ready(function(){

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
	}).on("message", function(evt){
		if(evt.data){
			$("#output").val([$("#output").val(), evt.data].join(""));
		}
	});
});