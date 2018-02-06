import React from 'react';
import Board from './Board.jsx';
import Color from './Color.jsx';
import PlayerColor from './PlayerColor.jsx';
import MoveTracker from './MoveTracker.jsx';
import CheatBox from './CheatBox.jsx';

import MOVE_TABLE from '../static/moveTable.js';

// r__rrrbr___r_bbr___r_br_b__b_bbb
// ___rrrb__r___brrb_r_b_rrbb_b___b

// r_rrr_r____r_rr____bbb___b_bbR_b

if (!Array.prototype.last){
    Array.prototype.last = function(){
        return this[this.length - 1];
    };
};

if (!String.prototype.replaceAt){
	String.prototype.replaceAt = function(idx,newChar){
		return `${this.substr(0,idx)}${newChar}${this.substr(idx+1)}`;
	};
};

function replaceAt(string,idx,newChar) {
	return `${string.substr(0,idx)}${newChar}${string.substr(idx+1)}`;
}

var sock;

const kingMe = {
	'red' : [29,30,31,32],
	'black' : [1,2,3,4]
}

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

function applyMoveToBoard(self,tile) {
	let newBoard = self.state.board.join('');

	if ( kingMe[self.state.turnColor].includes(tile) ) 
		newBoard = newBoard.replaceAt(tile-1, self.state.board[self.state.selectedTile-1].toUpperCase());
	else
		newBoard = newBoard.replaceAt(tile-1, self.state.board[self.state.selectedTile-1]);
	
	newBoard = newBoard.replaceAt(self.state.selectedTile-1, '_');

	return newBoard;
}

function applyJumpsToBoard(self,tile) {
	// console.log(self.state.jumpedTiles);
	let newBoard = self.state.board.join('');
	
	if ( kingMe[self.state.turnColor].includes(tile) )
		newBoard = replaceAt(newBoard, tile-1, self.state.board[self.state.selectedTile-1].toUpperCase());
	else
		newBoard = replaceAt(newBoard, tile-1, self.state.board[self.state.selectedTile-1]);

	self.state.jumpedTiles.forEach( val => {
		newBoard = replaceAt(newBoard, val-1, '_');
	});

	newBoard = replaceAt(newBoard, self.state.selectedTile-1,'_');

	return newBoard;
}

function tileHasNoAdjacentEnemies(board,turnColor,tile,king) {
	if (king)
		return (
			( MOVE_TABLE['red'][tile].reduce( (prev,curr) => {
				return prev && (board[curr-1] != (turnColor == 'red' ? 'b' : 'r') && board[curr-1] != (turnColor == 'red' ? 'B' : 'R') )
			}, true) )
			&&
			( MOVE_TABLE['black'][tile].reduce( (prev,curr) => {
				return prev && (board[curr-1] != (turnColor == 'red' ? 'b' : 'r') && board[curr-1] != (turnColor == 'red' ? 'B' : 'R') )
			}, true) ) );
	else
		return ( MOVE_TABLE[turnColor][tile].reduce( (prev,curr) => {
			return prev && (board[curr-1] != (turnColor == 'red' ? 'b' : 'r') && board[curr-1] != (turnColor == 'red' ? 'B' : 'R') )
		}, true) );
}

function tileHasNoJumpExits(self,board,turnColor,tile,king) {
	console.log("checking jump exits");
	let rval = null;
	if (king)
		rval = (
			( MOVE_TABLE['red-jump'][tile].reduce( (prev,curr) => {
				return prev && !tileIsValidJumpTarget(self, tile, curr, true, true);			
			}, true) )
			&&
			( MOVE_TABLE['black-jump'][tile].reduce( (prev,curr) => {
				return prev && !tileIsValidJumpTarget(self, tile, curr, true, true);			
			}, true) ) );
	else
		rval = ( MOVE_TABLE[`${turnColor}-jump`][tile].reduce( (prev,curr) => {
				return prev && !tileIsValidJumpTarget(self, tile, curr, true, false);			
			}, true) );

	return rval;
}

function otherColor(c) {
	return c == 'black' ? 'red' : 'black';
}

function tileBelongsToOpponent(board,tile,turnColor) {
	return board[tile-1] == (turnColor == 'red' ? 'b' : 'r') || board[tile-1] == (turnColor == 'red' ? 'B' : 'R');
}

function log(msg) {
	console.log(msg);
	return true;
}

function tileIsValidJumpTarget(self,lastTile,tile,check=false,king) {
	let jumpedTile = null;
	let r_val = null;
	if (!lastTile)
		lastTile = self.state.selectedTile;
	// console.log(self.state.selectedTile);
	console.log("validJump vars - from target",self.state.selectedTile,tile,false,isKing(self,self.state.selectedTile));
	console.log("validJump vars - as passed",lastTile,tile,check,king);
	console.log(`isValidMoveForKing: ${isValidMoveForKing(lastTile,tile,true)}`)
	if (king)
		r_val=((isValidMoveForKing(lastTile,tile,true) && self.state.board[tile-1] == '_' &&
				(	MOVE_TABLE['red'][lastTile].reduce( (prev,curr) => {
						if (tileBelongsToOpponent(self.state.board,curr,self.state.turnColor)) {
							console.log(`Tile: ${curr}  belongs to opponent`);
							if (isValidMoveForKing(curr,tile,false))
								jumpedTile = curr;
							console.log(`JumpedTile: ${jumpedTile}`);

							return prev || isValidMoveForKing(curr,tile,false);
						}
						else
							return prev || false;
				},false) || 
				MOVE_TABLE['black'][lastTile].reduce( (prev,curr) => {
					if (tileBelongsToOpponent(self.state.board,curr,self.state.turnColor)) {
						console.log(`Tile: ${curr} belongs to opponent`);
						if (isValidMoveForKing(curr,tile,false))
							jumpedTile = curr;
						console.log(`JumpedTile: ${jumpedTile}`);

						return prev || isValidMoveForKing(curr,tile,false);
					}
					else
						return prev || false;
				},false))));
	else 
		r_val=(	MOVE_TABLE[`${self.state.turnColor}-jump`][lastTile].includes(tile)  && 
				MOVE_TABLE[self.state.turnColor][lastTile].reduce( (prev,curr) => {
					if ( (	self.state.board[curr-1] == (self.state.turnColor == 'red' ? 'b' : 'r')  || 
							self.state.board[curr-1] == (self.state.turnColor == 'red' ? 'B' : 'R')) && 
							self.state.board[tile-1] == '_') {

						if (MOVE_TABLE[self.state.turnColor][curr].includes(tile))
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
		console.log(`jumped to ${tile} over ${jumpedTile}`);
	}

	return r_val;
}

function getOwnerColor(board,tile) {
	return 	( board[tile-1] == 'r' || board[tile-1] == 'R' ) ? 'red' : 
			( board[tile-1] == 'b' || board[tile-1] == 'B' ) ? 'black' : '';
}

function checkMoveAgainstServer(board,turnColor) {
	console.log();
	kingMe[turnColor].forEach( tile => {
		console.log(getOwnerColor(board,tile));
		// if (board[val-1] == )
		// 	console.log('king this: '+val);
		console.log(tile);
	})

	// if (board[kingMe[turnColor]])

	sock.send(`checkMove ${board} ${turnColor}`);
}

function isKing(self,tile) {
	console.log("checking king status");
	return self.state.board[tile-1] == 'B' || self.state.board[tile-1] == 'R';
}

function isValidMoveForKing(tile,targetTile,jump) {
	console.log("checking valid move for king");
	// jump should be true or false

	if (!jump) {
		if (MOVE_TABLE['red'][tile].includes(targetTile))
			return true;
		if (MOVE_TABLE['black'][tile].includes(targetTile))
			return true;
	}

	else if (jump) {
		if (MOVE_TABLE['red-jump'][tile].includes(targetTile))
			return true;
		if (MOVE_TABLE['black-jump'][tile].includes(targetTile))
			return true;
	}

	console.log("invalid move to adjacent tile")
	return false;
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
		console.log("clickEvent", element, tile)
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
			console.log("no owner color",element,tile)
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
				console.log(`catch#1 : selectedTile:${this.state.selectedTile}, clickedTile:${tile}`);

				if ( MOVE_TABLE[this.state.turnColor][this.state.selectedTile].includes(tile) ||
					(isKing(this,this.state.selectedTile) && isValidMoveForKing(this.state.selectedTile,tile,false)) ) {
					// valid move to adjacent tile

					let tempBoard = applyMoveToBoard(this,tile);

					checkMoveAgainstServer(tempBoard,this.state.turnColor);
					this.setState({selectedTile:null});
				}

				else if ( tileIsValidJumpTarget(this,this.state.selectedTile,tile,false,isKing(this,this.state.selectedTile)) || tileIsValidJumpTarget(this,this.state.jumpTargets.last(),tile,false,isKing(this,this.state.selectedTile) )) {
					// valid jump
					console.log(`catch#2 : selectedTile:${this.state.selectedTile}, clickedTile:${tile}`);
					
					if ( tileHasNoAdjacentEnemies(this.state.board,this.state.turnColor,tile,isKing(this,this.state.selectedTile)) )
					{
						// laneded on a tile without any adjacent enemy chips
						// send move to server
						// console.log("passed check");
						let newBoard = applyJumpsToBoard(this,tile);
						checkMoveAgainstServer(newBoard,this.state.turnColor);
						this.resetTileSelection();
					}

					else {
						// console.log("last checks");
						if(tileHasNoJumpExits(this,this.state.board,this.state.turnColor,tile,isKing(this,this.state.selectedTile))) {
							// console.log("can't jump away");
							let newBoard = applyJumpsToBoard(this,tile);
							checkMoveAgainstServer(newBoard,this.state.turnColor);
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
		console.log('reset tile selection');
		console.trace();
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
			checkMoveAgainstServer(move,this.state.turnColor);
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