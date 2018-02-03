import React from 'react';
import Board from './Board.jsx';
import Color from './Color.jsx';
import PlayerColor from './PlayerColor.jsx';
import MoveTracker from './MoveTracker.jsx';
import CheatBox from './CheatBox.jsx';

import MOVE_TABLE from '../static/moveTable.js';

if (!Array.prototype.last){
    Array.prototype.last = function(){
        return this[this.length - 1];
    };
};

var sock;

const baseState = {
	board: [],
	selectedTile: null, 
	jumpTargets: [],
	jumpedTiles: [],
	turnColor: 'red',
	playerColor: 'red',
	computerMoves: [],
}

const baseStateNoPlayerColor = {
	board: [],
	selectedTiles: [], 
	jumpTargets: [],
	jumpedTiles: [],
	turnColor: 'red',
	computerMoves: [],
}

function applyJumpsToBoard(self,tile) {
	let newBoard = self.state.board.join('');
	
	newBoard = replaceAt(newBoard, tile-1, self.state.board[self.state.selectedTile-1]);

	self.state.jumpedTiles.forEach( val => {
		newBoard = replaceAt(newBoard, val-1, '_');
	});

	newBoard = replaceAt(newBoard, self.state.selectedTile-1,'_');

	return newBoard;
}

function replaceAt(string,idx,newChar) {
	return `${string.substr(0,idx)}${newChar}${string.substr(idx+1)}`;
}

function tileHasNoAdjacentEnemies(board,turnColor,tile) {
	return ( MOVE_TABLE[turnColor][tile].reduce( (prev,curr) => {
		return prev && (board[curr-1] != (turnColor == 'red' ? 'b' : 'r') && board[curr-1] != (turnColor == 'red' ? 'B' : 'R') )
	}, true) );
}

function tileHasNoJumpExits(self,board,turnColor,tile) {
	// console.log('checking exits');
	let rval =  (
		MOVE_TABLE[`${turnColor}-jump`][tile].reduce( (prev,curr) => {
			
			
			console.log("curr: "+curr);
			console.log("validMove: "+tileIsValidJumpTarget(self, tile, curr, true));
			return prev && !tileIsValidJumpTarget(self, tile, curr, true);			
		}, true)
	)
	// console.log(rval);
	return rval;
}

function tileIsValidJumpTarget(self,lastTile,tile,check=false) {
	let jumpedTile = null;
	let r_val=(	MOVE_TABLE[`${self.state.turnColor}-jump`][lastTile].includes(tile)  && 
				MOVE_TABLE[self.state.turnColor][lastTile].reduce( (prev,curr) => {
					if ( (	self.state.board[curr-1] == (self.state.turnColor == 'red' ? 'b' : 'r')  || 
							self.state.board[curr-1] == (self.state.turnColor == 'red' ? 'B' : 'R')) && 
							self.state.board[tile-1] == '_') {

						jumpedTile = curr;
						return prev || MOVE_TABLE[self.state.turnColor][curr].includes(tile);
					}
					else 
						return prev ||  false;
				}, false) 
			);
	
	if ( !check && r_val && !self.state.jumpedTiles.includes(jumpedTile) ) {
		let jumpedTiles = self.state.jumpedTiles;
		jumpedTiles.push(jumpedTile);
		self.setState({
			jumpedTiles: jumpedTiles
		});
		// console.log(`jumped to ${tile} over ${jumpedTile}`);
	}

	return r_val;
}

export default class GameContainer extends React.Component {
	constructor(props) {
		super(props);

		this.state = baseState;

		this.clickTile = this.clickTile.bind(this);
		this.changePlayerColor = this.changePlayerColor.bind(this);
		this.resetBoard = this.resetBoard.bind(this);
		this.resetTileSelection = this.resetTileSelection.bind(this);
		this.submitMove = this.submitMove.bind(this);
		this.setBoardState = this.setBoardState.bind(this);
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
						let otherMoves = msg[3].split(',');
						let currentMoves = self.state.computerMoves;
						otherMoves.forEach( (val,key) => {
							currentMoves.splice(0,0,val);
						});

						self.setState(
							{
								board: board,
								turnColor: nextPlayer,
								computerMoves: currentMoves
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
			else if (this.state.jumpTargets.last() == tile ) {
				let jumpTargets = this.state.jumpTargets;
				jumpTargets.splice(-1,1);
				let jumpedTiles = this.state.jumpedTiles;
				jumpedTiles.splice(-1,1);
				this.setState({
					jumpTargets: jumpTargets,
					jumpedTiles: jumpedTiles
				});
			}

			else {
				if ( MOVE_TABLE[this.state.turnColor][this.state.selectedTile].includes(tile)) {
					// valid move to adjacent tile
					// console.log("valid move");

					let tempBoard = this.state.board.join('');
					let tempVar = tempBoard[this.state.selectedTile-1];

					tempBoard = replaceAt(tempBoard, this.state.selectedTile-1, this.state.board[tile-1]);
					tempBoard = replaceAt(tempBoard, tile-1, tempVar);

					sock.send(`checkMove ${tempBoard} ${this.state.turnColor}`);
					this.setState({selectedTile:null});
				}

				else if ( tileIsValidJumpTarget(this,this.state.selectedTile,tile) || tileIsValidJumpTarget(this,this.state.jumpTargets.last(),tile )) {
					// valid jump
					
					if ( tileHasNoAdjacentEnemies(this.state.board,this.state.turnColor,tile) )
					{
						// laneded on a tile without any adjacent enemy chips
						// send move to server
						// console.log("passed check");
						let newBoard = applyJumpsToBoard(this,tile);
						sock.send(`checkMove ${newBoard} ${this.state.turnColor}`);

						this.resetTileSelection();
					}

					else {
						// console.log("last checks");
						if(tileHasNoJumpExits(this,this.state.board,this.state.turnColor,tile)) {
							// console.log("can't jump away");
							let newBoard = applyJumpsToBoard(this,tile);
							sock.send(`checkMove ${newBoard} ${this.state.turnColor}`);
							this.resetTileSelection();
						}

						else {
							let jumps = this.state.jumpTargets;
							jumps.push(tile);
							this.setState({
								jumpTargets: jumps
							}, () => {
								// console.log("jumpTargets: "+this.state.jumpTargets);
							});
							// console.log("lastJumpTarget: "+this.state.jumpTargets.last());
						}						
					}
					
					// valid jump target
				}

				else {
					console.log("invalid move");
				}
			}
		}

		else
			console.log("You can't move that tile!");
	}

	resetTileSelection() {
		this.setState({
			selectedTile: null,
			jumpTargets: [],
			jumpedTiles: []
		});
	}

	resetBoard() {
		this.setState(baseStateNoPlayerColor,
		() => {
			sock.send(`resetGame`);
		});
	}

	changePlayerColor(color) {
		this.setState(
			{playerColor:color}, 
			() => {
				this.resetBoard();
				sock.send(`changePlayerColorTo ${color}`);
			}
		);
	}

	submitMove(move) {
		if (move != "" && move.length == 32)
			sock.send(`checkMove ${move} ${this.state.turnColor}`);
		else
			console.log("invalid board string");
	}

	setBoardState(move) {
		console.log(move.length);
		if (move != "" && move.length == 32) {
			this.setState({
				board: move.split('')
			});
		}
		else
			console.log("invalid board string");
	}

	render(){
		return (
			<div>
				<h1>Checkers AI</h1>
				<button onClick={this.resetBoard}>
					Reset Board
				</button>
				<button onClick={this.resetTileSelection}>
					Reset Tile Selections
				</button>
				<PlayerColor 
					playerColor={this.state.playerColor} 
					changePlayerColor={this.changePlayerColor} 
				/>
				<Board 
					board={this.state.board} 
					clickTile={this.clickTile} 
					selectedTile={this.state.selectedTile} 
					jumpTargets={this.state.jumpTargets}
				/>
				<Color 
					className="color turnMarker" 
					color={this.state.turnColor} 
				/>
				<MoveTracker computerMoves={this.state.computerMoves} />
				<CheatBox label="Test Cheat" submitMove={this.submitMove} />
				<CheatBox label="Change Board" submitMove={this.setBoardState} />
			</div>
		);
	}
}