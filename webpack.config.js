const path = require('path');
const webpack = require('webpack');
const HtmlWebpackPlugin = require('html-webpack-plugin');

module.exports = {
	entry: './client/index.js',
	output: {
		path: path.resolve('./'),
		filename: 'bundle.js'
	},
	module: {
		rules: [
			{ 
				test: /\.js$/, 
				use: [
					{ loader: 'babel-loader'}
				],
				exclude: /node_modules/ 
			},
			{ 
				test: /\.jsx$/, 
				use: [
					{ loader: 'babel-loader' }
				], 
				exclude: /node_modules/ 
			},
			{ 
				test: /\.css$/, 
				use: [ "style-loader", "css-loader" ]
			}
		]
	},
	plugins: [
		new webpack.optimize.OccurrenceOrderPlugin(),
		new webpack.optimize.UglifyJsPlugin({
			compressor: {
				warnings: false,
			},
		}),
		new HtmlWebpackPlugin({
			template: './client/index.html',
			filename: 'index.html',
			inject: 'body',
		}),
	],
	watch: true
}