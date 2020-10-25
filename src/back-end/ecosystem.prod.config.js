module.exports = {
  apps : [{
    name: 'hacker-backend',
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
      PORT:9001,
    }
  }],
}

