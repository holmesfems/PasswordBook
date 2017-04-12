/*
 * password_service.js
 * Copyright (C) 2017 Jerry Jia <jerryjiahaha@gmail.com>
 *
 * Distributed under terms of the MIT license.
 */
'use strict';

const PROTO_PATH = __dirname + '/password_service.proto';

const grpc = require('grpc');
console.info(PROTO_PATH);
const password_service_proto = grpc.load(PROTO_PATH).passwordservice;

(function(){
	const client = new password_service_proto.PasswordService('localhost:23333', grpc.credentials.createInsecure());

	client.listItems()
	.on('data', function(item) {
		console.log("get item:", item);
	})
	.on('end', function() {
		console.log("read fin");
	})
	.on('status', function(status) {
		console.log("status:", status);
	})
})();

