/*
 * index.js
 * Copyright (C) 2017 Jerry Jia <jerryjiahaha@gmail.com>
 *
 * Distributed under terms of the MIT license.
 */
'use strict';
//const vue = require('vue');
import Vue from 'vue';
import axios from 'axios';
import regeneratorRuntime from 'regenerator-runtime';

const app = new Vue({
  el: "#app",
  data: {
    items: {},
  },
  methods: {
    listItems: async function() {
      console.log(this);
      try {
        const res = await axios.get(`/api/listItems`);
        console.log(`res: ${Object.entries(res)}`);
        console.log(res.data);
        this.items = res.data;
      } catch (e) {
        console.error(e);
      }
    }
  }
});


// vim: set ts=2 sw=2 et:
