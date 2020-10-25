
const fs = require('fs')
const path = require('path')
const withLess = require('@zeit/next-less')
const lessToJS = require('less-vars-to-js')
require('dotenv').config()

// Where your antd-custom.less file lives
const themeVariables = lessToJS(
  fs.readFileSync(path.resolve(__dirname, './assets/custom-less/antd-custom.less'), 'utf8')
)

module.exports = withLess({
  env: {
    APIV1_DEV: process.env.APIV1_DEV,
    APIV1_PROD: process.env.APIV1_PROD,
    APIV1: process.env.APIV1,
    API_TOKEN: process.env.API_TOKEN,
    MQTT_HOST: process.env.MQTT_HOST,
    VERIFY_GATEWAY: process.env.VERIFY_GATEWAY,
    VERIFY_HOST: process.env.VERIFY_HOST,
    PORT: process.env.PORT,
  },

  cssLoaderOptions: {
    importLoaders: 1,
    localIdentName: "[local]___[hash:base64:5]",
  },
  lessLoaderOptions: {
    javascriptEnabled: true,
    modifyVars: themeVariables, // make your antd custom effective
  },

  webpack: (config, { isServer, dev }) => {
    if (isServer) {
      const antStyles = /antd\/.*?\/style.*?/
      const origExternals = [...config.externals]
      config.externals = [
        (context, request, callback) => {
          if (request.match(antStyles)) return callback()
          if (typeof origExternals[0] === 'function') {
            origExternals[0](context, request, callback)
          } else {
            callback()
          }
        },
        ...(typeof origExternals[0] === 'function' ? [] : origExternals),
      ]

      config.module.rules.unshift({
        test: antStyles,
        use: 'null-loader',
      })
    }

    if (dev) { // try to speed up next building when dev.
      config.devtool = 'cheap-module-source-map';
    }

    return config
  },
})


