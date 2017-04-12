/*
 * index.js
 * Copyright (C) 2017 Jerry Jia <jerryjiahaha@gmail.com>
 *
 * Distributed under terms of the MIT license.
 */
'use strict';
//const vue = require('vue');
import Vue from 'vue';

const app = new Vue({
	el: "#app",
	data: {
		items: {},
	},
	methods: {
		listItems: function() {
			console.log(this);
			this.items = [
				{id: 1, domain: "ip.cn"},
				{id: 2, domain: "google.com"},
			];
		}
	}
});
