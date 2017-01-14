var express = require('express');
var path = require("path");
var port = (process.env.VCAP_APP_PORT || 3000);
var host = (process.env.VCAP_APP_HOST || 'localhost');
var app = express();


app.use(express.static(path.join(__dirname,'public')));
app.get('/',function(req,res){
  res.sendFile(path.join(__dirname, 'public/index.html'));
});


app
.listen(port, host,function(){
	console.log('Running on PORT : ' + port);
});
