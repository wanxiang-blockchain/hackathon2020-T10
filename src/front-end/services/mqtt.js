const mqtt = require('mqtt')

const host = process.env.MQTT_HOST 
let client = null;
let messageCount = 1

export function start(name, onMessage, onError) {
  const clientId = (name ?  name : 'clientUnknown_')+ Math.random().toString(16).substr(2, 8);

  var options = {
    keepalive: 30,
    clientId : clientId,
    protocolId: 'MQTT',
    protocolVersion: 4,
    clean: true,
    reconnectPeriod: 1000,
    connectTimeout: 30 * 1000,
    username: 'broker01',
    password: 'iot123',
    will: {
      topic: 'WillMsg',
      payload: 'Connection Closed abnormally..!',
      qos: 0,
      retain: false
    },
    // username: 'xinfa',
    // password: 'qaz123456',
    rejectUnauthorized: true
  }


  client = mqtt.connect(host, options);

  onMessage = onMessage || ((topic, message) => console.log('Received Message:= ' + message.toString() + '\nOn topic:= ' + topic)) ;

  client.on('error', function (err) {
    console.log(err)
    client.end()
  })
  
  client.on('connect', function () {
    console.log('client connected:' + clientId)
  })
  
  client.subscribe('topic', { qos: 0 })
  
  
  client.on('message', function (topic, message, packet) {
    console.log('[mqtt] Message:= ' + message.toString() + '\nOn topic:= ' + topic);
    onMessage(topic, message.toString());
  })
  
  client.on('close', function () {
    console.log(clientId + ' disconnected')
  })
}

export function sendTo(target, message, cb) {

  if (!client) {
    cb && cb('error', 'invalid mqttClinet')
    return;
  }
  console.log(`[mqtt]publish topic: ${message}`);
  client.publish('topic', message, { qos: 0, retain: false })
}


export function publish(topic, message, cb) {
  if (!client) {
    cb && cb('error', 'invalid mqttClinet')
    return;
  }

  console.log(`[mqtt]publish ${topic}: ${message}`);
  client.publish(topic, message, { qos: 0, retain: false })
}

export function subscribe(topic) {
  if (!client) {
    cb && cb('error', 'invalid mqttClinet')
    return;
  }

  client.subscribe(topic, { qos: 0 })
}


export function createClient(name, topic, onMessage) {

  const clientId = (name ?  name : 'clientUnknown_')+ Math.random().toString(16).substr(2, 8);

  var options = {
    keepalive: 30,
    clientId : clientId,
    protocolId: 'MQTT',
    protocolVersion: 4,
    clean: true,
    reconnectPeriod: 1000,
    connectTimeout: 30 * 1000,
    username: 'broker01',
    password: 'iot123',
    will: {
      topic: 'WillMsg',
      payload: 'Connection Closed abnormally..!',
      qos: 0,
      retain: false
    },
    // username: 'xinfa',
    // password: 'qaz123456',
    rejectUnauthorized: true
  }


  let _client = mqtt.connect(host, options);

  onMessage = onMessage || ((topic, message) => console.log('Received Message:= ' + message.toString() + '\nOn topic:= ' + topic)) ;

  _client.on('error', function (err) {
    console.log(err)
    _client.end()
  })
  
  _client.on('connect', function () {
    console.log('_client connected:' + clientId)
  })
  
  _client.subscribe(topic, { qos: 0 })
  

  _client.on('message', function (topic, message, packet) {
    //    console.log(`[mqtt]${messageCount} Message:= ` + message.toString() + '\nOn topic:= ' + topic);
     messageCount = messageCount + 1

    onMessage(topic, message.toString());
  })
  
  _client.on('close', function () {
    console.log(clientId + ' disconnected')
  })
}

export function closeClient( _client) {
  _client && _client.end()
}



