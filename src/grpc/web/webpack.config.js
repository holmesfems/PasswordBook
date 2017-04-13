const webpack = require('webpack');
const ManifestPlugin = require('webpack-manifest-plugin');
const path = require('path');

// ref: https://webpack.js.org/guides/code-splitting-libraries/

module.exports = {
  resolve: {
    alias: {
      'vue$': 'vue/dist/vue.esm.js',
    },
  },
  entry: {
    main: './app/index.js',
    axios: 'axios',
    vue: 'vue',
  },
  output: {
    //filename: '[name].[chunkhash].js',
    filename: '[name].js',
    path: path.resolve(__dirname, 'dist'),
    pathinfo: true,
  },
  devtool: 'sourcemap',
  module: {
    rules: [
      {
        test: /\.js$/,
        exclude: /(node_modules|bower_components)/,
        use: {
          loader: 'babel-loader',
          options: {
           // presets: ['env'],
            cacheDirectory: true,
            //						plugins: [require('babel-plugin-transform-object-rest-spread'), require('babel-plugin-transform-async-to-generator')]
          }
        }
      }
    ]
  },
  plugins: [
    new webpack.optimize.CommonsChunkPlugin({
      name: 'vue',
      minChunks: (module) => {
        return module.context && module.context.indexOf('node_modules') !== -1;
      }
    }),
//    new webpack.optimize.CommonsChunkPlugin({
//      name: 'axios',
//      minChunks: (module) => {
//        return module.context && module.context.indexOf('node_modules') !== -1;
//      }
//    }),
    //CommonChunksPlugin will now extract all the common modules from vendor and main bundles
    new webpack.optimize.CommonsChunkPlugin({
      name: 'manifest', //But since there are no more common modules between them we end up with just the runtime code included in the manifest file
    }),
    new ManifestPlugin({
      // https://www.npmjs.com/package/webpack-manifest-plugin
      //fileName: 'my-manifest.json',
      //basePath: '/app/'
    }),
    //		new webpack.DefinePlugin({
    //			'process.env': {
    //				NODE_ENV: '"production"'
    //			}
    //		}),
  ],
}

// vim: set ts=2 sw=2 et:
