const TILES = { 
	'_': '', 
	'b': 'b', 
	'B': 'bk', 
	'r': 'r', 
	'R': 'rk'
}

class GameContainer extends React.Component {
	constructor(props) {
		super(props);

		this.state = {
			board: [],
			selectedTile: null, 
			turn: 'red',
			playerColor: 'black'
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
						console.log(board);
						reactParent.setState({board:board});
					}
				}
			}
			
		}
	}

	clickTile(element, tile) {
		let ownerColor = element.classList.contains('red') ? 'red' : 'black';

		if (this.state.selectedTile == null) {

			this.setState({selectedTile:tile});
		}
		else if (this.state.selectedTile == tile)
			this.setState({selectedTile:null});
		else {
			sock.send(`checkMove ${this.state.selectedTile} ${tile}`);
			this.setState({selectedTile:null});
		}

	}

	changePlayerColor(element) {
		console.log(element);
	}

	render(){
		

		return (
			<div>
				<Board board={this.state.board} clickTile={this.clickTile} focus={this.state.selectedTile} />
				<div className="playerColor">
					<input onChange={(event) => this.changePlayerColor(event.target)} type="radio" id="red" name="playerColor" value="red" />
					
					<input onChange={(event) => this.changePlayerColor(event.target)} type="radio" id="black" name="playerColor" value="black" />
				</div>
				<div className="turnMarker" style={{backgroundColor: this.state.turn}}></div>
			</div>
			
		);
	}
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
					className={`green ${TILES[val][0] == 'r' ? 'red' : 'black'} ${TILES[val]} ${props.focus == (key+1) ? 'focus' : ''}`}
					onClick={(event) => props.clickTile(event.target,key+1)}>
					<p>{key+1}</p>
				</td>);
			boardRow.push(<td key={`${key}_`} className="buff"></td>);
		}
		else {
			boardRow.push(<td key={`${key}_`} className="buff"></td>);
			boardRow.push(
				<td key={key} 
					className={`green ${TILES[val][0] == 'r' ? 'red' : 'black'} ${TILES[val]} ${props.focus == (key+1) ? 'focus' : ''}`}
					onClick={(event) => props.clickTile(event.target,key+1)}>
					<p>{key+1}</p>
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