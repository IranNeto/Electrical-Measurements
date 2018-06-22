const path = require('path')
const http     = require('http')
const express  = require('express')
const app      = express();
const httpServer = http.createServer(app)
const mosca    = require('mosca')

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
  /*
    É aqui que será necessário formatar o dado recebido do esp para um vetor de números e aplicar a fft
    Acho que a biblioteca que vc me mandou no wpp é a msm que usei pra testar é só seguir os exemplos que dá certo (vê app2.js)
  */
  console.log('Published', packet.payload.toString())
});

broker.on('ready', () => console.log('Mosca server is up and running'))
