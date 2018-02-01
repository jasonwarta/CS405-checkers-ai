import React from 'react';

import TILES from '../static/tiles.js'

export default function Board(props) {
	let boardArray = [];
	let toggle = true;
	let boardRow = [];
	let rowCounter = 0;
	props.board.forEach( (val,key) => {
		if (toggle) {
			boardRow.push(<td key={`${key}_`} className="buff"></td>);
			boardRow.push(
				<td key={key} 
					className={`green ${TILES[val][1]} ${TILES[val][0]} ${props.focus == (key+1) ? 'focus' : ''}`}
					onClick={(event) => props.clickTile(event.target,key+1)}>
					<p className="tileLabel">{key+1}</p>
				</td>);
		}
		else {
			boardRow.push(
				<td key={key} 
					className={`green ${TILES[val][1]} ${TILES[val][0]} ${props.focus == (key+1) ? 'focus' : ''}`}
					onClick={(event) => props.clickTile(event.target,key+1)}>
					<p className="tileLabel">{key+1}</p>
				</td>);
			boardRow.push(<td key={`${key}_`} className="buff"></td>);
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