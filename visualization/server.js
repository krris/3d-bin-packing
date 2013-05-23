var http = require('http');
var fs = require('fs');
var sys = require('sys');

http.createServer(function(req, res){
        fs.readFile('cubes.html',function (err, data){
                    res.writeHead(200, {'Content-Type': 'text/html','Content-Length':data.length});
                            res.write(data);
                                    res.end();
                                        });
}).listen(8888);
