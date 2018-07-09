const path = require('path')
const http     = require('http')
const express  = require('express')
const app      = express();
const httpServer = http.createServer(app)
const mosca    = require('mosca')
var us = require('microtime');
var us_time = us.now();
//const fft = require('fft-js').fft,
// fftUtil = require('fft-js').util, signal[];

/*
const ascoltatore = {
  //using ascoltatore
  type: 'mongo',
  url: 'mongodb://localhost:27017/mqtt',
  pubsubCollection: 'ascoltatori',
  mongo: {}
};
*/

const broker = new mosca.Server({port: 8088});

broker.on('clientConnected', (client) => {
  console.log('client connected', client.id)
});

// fired when a message is received/js
broker.on('published', (packet, client) => {
  console.log('Published', packet.payload.toString())
  //pegar dados do campo "dados" do json e formatar como um vetor de inteiros pra fazer a operação de fft
});

broker.on('ready', () => console.log('Mosca server is up and running'))
