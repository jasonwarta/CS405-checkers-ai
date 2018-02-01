import React from 'react';
import Board from './Board.jsx';
import Color from './Color.jsx';
import PlayerColor from './PlayerColor.jsx';

import MOVE_TABLE from '../static/moveBoard.js'

var sock;

function replaceAt(string,idx,newChar) {
	return `${string.substr(0,idx)}${newChar}${string.substr(idx+1)}`;
}

export default class GameContainer extends React.Component {
	constructor(props) {
		super(props);

		this.state = {
			board: [],
			selectedTile: null, 
			turnColor: 'red',
			playerColor: 'red'
		}

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
		}

		else if (ownerColor == '') {
			if(this.state.selectedTile == null) {
				console.log("You must select a piece first");
			} else {
				let tempBoard = this.state.board.join('');
				let tempVar = tempBoard[this.state.selectedTile-1];
				tempBoard = replaceAt(tempBoard, this.state.selectedTile-1, this.state.board[tile-1]);
				tempBoard = replaceAt(tempBoard, tile-1, tempVar);

				sock.send(`checkMove ${tempBoard} ${this.state.turnColor}`);
				this.setState({selectedTile:null});
			}
		}

		else 
			console.log("You can't move that tile!");
	}

	resetBoard() {
		this.setState({
			turnColor: 'red'
		},
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