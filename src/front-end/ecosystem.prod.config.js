module.exports = {
  apps : [{
    name: 'frontend',
    script: 'npm',

    // Options reference: https://pm2.io/doc/en/runtime/reference/ecosystem-file/
    args: 'start',
    instances: 1,
    autorestart: true,
    log_date_format: "YYYY-MM-DD HH:mm Z",
    error_file: 'err.log',
    out_file: 'out.log',
    log_file: 'combined.log',   
    max_memory_restart: '1G',
    env: {
      NODE_ENV: 'production'
    },
    env_production: {
      NODE_ENV: 'production',
      APIV1: 'http://172.25.0.231:9001/',
      API_TOKEN: 'ci_token01',
      MQTT_HOST: 'ws://172.25.0.231:8883',
      VERIFY_GATEWAY: 'http://172.25.0.231:9005/',
      PORT:9002,
    }
  }],
}

