#!/usr/bin/env node

var http = require('http');
var querystring = require('querystring');

http.createServer(function (req, res) {
    var values =  req.url.match(/\/(\d+)\+(\d+)/);
    var query = querystring.stringify({a: values[1], b: values[2]}); 

    http.get({hostname:'localhost', port:3000, path: '/add?' + query}, function(apiRes) {
        
        apiRes.on('data', function (content) {
            res.writeHead(200, {'Content-Type': 'text/plain'});
            res.end(content);
        });
    });
}).listen(8000, '0.0.0.0');
