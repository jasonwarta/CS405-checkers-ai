import React from 'react';
import ReactDOM from 'react-dom';

const TILES = { 
	'_': ['',''], 
	'b': ['b','black'], 
	'B': ['bk','black'], 
	'r': ['r','red'], 
	'R': ['rk','red']
}

function replaceAt(string,idx,newChar) {
	return `${string.substr(0,idx)}${newChar}${string.substr(idx+1)}`;
}

class GameContainer extends React.Component {
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
	}

	componentWillMount() {
		let reactParent = this;
		sock = new WebSocket(`ws://${window.location.hostname}:${window.location.port}`);

		sock.onmessage = function(event) {
			if(event){
				if (event.data) {
					let msg = event.data.split(' ');

					if(msg[0] == "board") {
						let board = msg[1].split('');
						reactParent.setState({board:board});
					}
				}
			}
		}
	}

	clickTile(element, tile) {
		let ownerColor = '';
		if (element.classList.contains('red'))
			ownerColor = 'red';
		else if (element.classList.contains('black'))
			ownerColor = 'black';

		console.log(`ownerColor: ${ownerColor}`);

		if (this.state.turnColor == ownerColor && this.state.turnColor == this.state.playerColor) {
			if (this.state.selectedTile == null) {
				this.setState({selectedTile:tile});
			}
			else if (this.state.selectedTile == tile) {
				this.setState({selectedTile:null});
			}
		}
		else if (ownerColor == '') {
			let tempBoard = this.state.board.join('');
			let tempVar = tempBoard[this.state.selectedTile-1];
			tempBoard = replaceAt(tempBoard, this.state.selectedTile-1, this.state.board[tile-1]);
			tempBoard = replaceAt(tempBoard, tile-1, tempVar);

			sock.send(`checkMove ${tempBoard} ${this.state.turnColor}`);
			this.setState({selectedTile:null});
		}

		else 
			console.log("You can't move that tile!");
	}

	changePlayerColor(color) {
		this.setState(
			{playerColor:color}, 
			() => {
				sock.send(`resetGame`);
			}
		);
	}

	render(){
		return (
			<div>
				<Board board={this.state.board} clickTile={this.clickTile} focus={this.state.selectedTile} />
				<div className="playerColor">
					<label>
						<input checked={`${this.state.playerColor == 'red' ? 'true' : ''}`} onChange={(event) => this.changePlayerColor(event.target.value)} type="radio" id="red" name="playerColor" value="red" />
						<Color className="color" color="red"/>
					</label>
					<label>
						<input checked={`${this.state.playerColor == 'black' ? 'true' : ''}`} onChange={(event) => this.changePlayerColor(event.target.value)} type="radio" id="black" name="playerColor" value="black" />
						<Color className="color" color="black"/>
					</label>
				</div>
				<Color className="color turnMarker" color={this.state.turnColor} />
			</div>
		);
	}
}

function Color(props) {
	return (
		<div style={{backgroundColor: props.color}} {...props} ></div>
	)
}

function Board(props) {
	let boardArray = [];
	let toggle = true;
	let boardRow = [];
	let rowCounter = 0;
	props.board.forEach( (val,key) => {
		if (toggle) {
			boardRow.push(
				<td key={key} 
					className={`green ${TILES[val][1]} ${TILES[val][0]} ${props.focus == (key+1) ? 'focus' : ''}`}
					onClick={(event) => props.clickTile(event.target,key+1)}>
					<p className="tileLabel">{key+1}</p>
				</td>);
			boardRow.push(<td key={`${key}_`} className="buff"></td>);
		}
		else {
			boardRow.push(<td key={`${key}_`} className="buff"></td>);
			boardRow.push(
				<td key={key} 
					className={`green ${TILES[val][1]} ${TILES[val][0]} ${props.focus == (key+1) ? 'focus' : ''}`}
					onClick={(event) => props.clickTile(event.target,key+1)}>
					<p className="tileLabel">{key+1}</p>
				</td>);
		}
		if(boardRow.length == 8) {
			boardArray.push(<tr key={`r${rowCounter++}`}>{boardRow}</tr>);
			toggle = !toggle;
			boardRow = [];
		}
	});

	return (
		<table>
			<tbody>{boardArray}</tbody>
		</table>
	);
}

ReactDOM.render(
	<GameContainer />,
	document.getElementById('root')
);