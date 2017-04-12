/*
 * server.js
 * Copyright (C) 2017 Jerry Jia <jerryjiahaha@gmail.com>
 *
 * Distributed under terms of the MIT license.
 */
'use strict';

const logger = require('koa-logger');
const Koa = require('koa');
//const middleware = require('koa-webpack');
const render = require('koa-ejs');
const Router = require('koa-router');
const path = require('path');
const serve = require('koa-static');
const mount = require('koa-mount');

const app = new Koa();
const router = new Router();

//app.use(middleware());
render(app, {
	root: path.join(__dirname, 'html'),
	layout: false,
	viewExt: 'html',
	cache: false,
	debug: true
});
app.use(logger());

router.use(
	async (ctx, next) => {
		const start = new Date();
		await next();
		const ms = new Date() - start;
		ctx.set('X-Response-Time', `${ms}`);
		console.log(`${ctx.method} ${ctx.url} - ${ms}`);
	});
router
	.get('/', async ctx => {
		await ctx.render('index');
	})
	.get('/test', async ctx => {
		await ctx.render('test');
	})
	.get('/api/listitem', async ctx => {
		ctx.body = {};
	})
app
	.use(router.routes())
	.use(router.allowedMethods());

function add_static(app, _url, _static) {
	const app_static = new Koa();
	app_static.use(serve(_static));
	app.use(mount(_url, app_static));
}
add_static(app, '/dist', 'dist');

//// x-response-time
//app.use(async function (ctx, next) {
//	const start = new Date();
//	await next();
//	const ms = new Date() - start;
//	ctx.set('X-Response-Time', `${ms}ms`);
//	console.log(`${ctx.method} ${ctx.url} - ${ms}`);
//});

//app.use(async function (ctx) {
//	await ctx.render('index');
//});

//app.use(ctx => {
//	ctx.body = 'Hello world';
//});

app.listen(3000);
