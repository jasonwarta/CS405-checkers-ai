import React from 'react';
import Board from './Board.jsx';
import Color from './Color.jsx';
import PlayerColor from './PlayerColor.jsx';

import MOVE_TABLE from '../static/moveTable.js'

var sock;

const baseState = {
	board: [],
	selectedTile: null, 
	jumpTiles: [],
	turnColor: 'red',
	playerColor: 'red'
}

function replaceAt(string,idx,newChar) {
	return `${string.substr(0,idx)}${newChar}${string.substr(idx+1)}`;
}

export default class GameContainer extends React.Component {
	constructor(props) {
		super(props);

		this.state = baseState;

		this.clickTile = this.clickTile.bind(this);
		this.changePlayerColor = this.changePlayerColor.bind(this);
		this.resetBoard = this.resetBoard.bind(this);
	}

	componentWillMount() {
		let self = this;
		sock = new WebSocket(`ws://${window.location.hostname}:${window.location.port}`);

		sock.onmessage = function(event) {
			if(event){
				if (event.data) {
					let msg = event.data.split(' ');
					let board = msg[1].split('');
					console.log(msg);

					if(msg[0] == 'board') {
						self.setState({board:board});
					}

					else if (msg[0] == 'confirmMove') {
						let nextPlayer = msg[2] == 'red' ? 'black' : 'red';
						self.setState(
							{
								board: board,
								turnColor: nextPlayer
							},
							() => {
								sock.send(`computerMove ${self.state.board.join('')} ${nextPlayer}`);
							}
						)
					}

					else if (msg[0] == 'move') {
						let nextPlayer = msg[2] == 'red' ? 'black' : 'red';
						self.setState(
							{
								board: board,
								turnColor: nextPlayer
							}
						)
					}
				}
			}
		};
	}

	clickTile(element, tile) {
		let ownerColor = '';
		if (element.classList.contains('red'))
			ownerColor = 'red';
		else if (element.classList.contains('black'))
			ownerColor = 'black';


		if (this.state.turnColor == ownerColor && this.state.turnColor == this.state.playerColor) {
			if (this.state.selectedTile == null) {
				this.setState({selectedTile:tile});
			}

			else if (this.state.selectedTile == tile) {
				this.setState({selectedTile:null});
			}

			else {
				console.log(`Unhandled state!\nElement:\n${element}\nTile: ${tile}\nState:\n${this.state}`);
			}
		}

		else if (ownerColor == '') {
			if (this.state.selectedTile == null) {
				console.log("You must select a piece first");
			}

			else {
				if ( MOVE_TABLE[this.state.turnColor][this.state.selectedTile].includes(tile)) {
					// valid move to adjacent tile
					console.log("valid move");

					let tempBoard = this.state.board.join('');
					let tempVar = tempBoard[this.state.selectedTile-1];

					tempBoard = replaceAt(tempBoard, this.state.selectedTile-1, this.state.board[tile-1]);
					tempBoard = replaceAt(tempBoard, tile-1, tempVar);

					sock.send(`checkMove ${tempBoard} ${this.state.turnColor}`);
					this.setState({selectedTile:null});
				}

				else if(MOVE_TABLE[`${this.state.turnColor}-jump`][this.state.selectedTile].includes(tile) && 
						MOVE_TABLE[this.state.turnColor][this.state.selectedTile].reduce( (prev,curr) => {
							if(	this.state.board[curr-1] == (this.state.turnColor == 'red' ? 'b' : 'r') || 
								this.state.board[curr-1] == (this.state.turnColor == 'red' ? 'B' : 'R') )
								return prev || MOVE_TABLE[this.state.turnColor][curr].includes(tile);
							else 
								return prev ||  false;
						}, false) ) {

					// valid jump target
					console.log("valid jump");
				}

				else {
					console.log("invalid move");
				}
			}
		}

		else
			console.log("You can't move that tile!");
	}

	resetBoard() {
		this.setState(baseState,
		() => {
			sock.send(`resetGame`);
		});
	}

	changePlayerColor(color) {
		this.setState(
			{playerColor:color}, 
			() => {
				this.resetBoard();
			}
		);
	}

	render(){
		return (
			<div>
				<h1>Checkers AI</h1>
				<button 
					onClick={this.resetBoard}>Reset Board</button>
				<PlayerColor 
					playerColor={this.state.playerColor} 
					changePlayerColor={this.changePlayerColor} 
				/>
				<Board 
					board={this.state.board} 
					clickTile={this.clickTile} 
					focus={this.state.selectedTile} 
				/>
				<Color 
					className="color turnMarker" 
					color={this.state.turnColor} 
				/>
			</div>
		);
	}
}