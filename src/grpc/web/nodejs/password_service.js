/*
 * password_service.js
 * Copyright (C) 2017 Jerry Jia <jerryjiahaha@gmail.com>
 *
 * Distributed under terms of the MIT license.
 */

// TODO use es6 import/export instead of require/exports

const path = require('path');

// settings
// TODO set them as an options
const PROTO_FILE = 'password_service.proto';
const HOST = 'localhost:23333';

const PROTO_PATH = path.join(path.join(__dirname, `../../`) ,`${PROTO_FILE}`);

const grpc = require('grpc');

try {
  const grpc_proto = grpc.load(PROTO_PATH);
  console.log(`proto ${PROTO_PATH} loaded`);
  const password_service_proto = grpc_proto.passwordservice;
} catch (err) {
  console.error(err);
  console.error(`proto file ${PROTO_PATH} not found or 'passwordservice' not found`);
  module.exports = {};
  return;
}

// TODO do not call grpc.load twice...
const grpc_proto = grpc.load(PROTO_PATH);
const password_service_proto = grpc_proto.passwordservice;
const client = new password_service_proto.PasswordService(HOST, grpc.credentials.createInsecure());

/* XXX I have to deal with `this`
function curry(func, ...params) {
  return () => {
    console.log("params:", params);
    func(...params);
  }
}
*/

function grpc_api_read_stream(apifunc) {
  return new Promise((resolve, reject) => {
    let data = [];
    const call = apifunc()
      .on('data', (response) => {
        console.log("res:", response);
        data.push(response);
      })
      .on('end', () => {
        console.log("end");
        resolve(data);
      })
      .on('status', (status) => {
        console.log("status:", status);
        if (status.code != 0) {
          reject(new Error(status.details));
        }
      })
      .on('error', (err) => {
        console.error(err);
      });
  });
}

function ApiNotFoundException(value) {
  this.value = value;
  this.message = `Api_Not_Found ${value}`;
  this.name = `ApiNotFoundException`;
}

module.exports = {
  api: async (api, ...params) => {
    if (Object.keys(Object.getPrototypeOf(client)).indexOf(api) === -1) {
      console.error(`api not found: ${api}`);
      throw new ApiNotFoundException(api);
    }
    const got = await grpc_api_read_stream(
      () => client[api](...params)
    );
    return got;
  }
};

// vim: set ts=2 sw=2 et:
