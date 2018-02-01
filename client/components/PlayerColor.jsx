import React from 'react';
import Color from './Color.jsx';

export default function PlayerColor(props) {
	return (
		<div className="playerColor">
			<span className="playerColorLabel">Player Color: </span>
			<label>
				<input checked={`${props.playerColor == 'red' ? 'true' : ''}`} onChange={(event) => props.changePlayerColor(event.target.value)} type="radio" id="red" name="playerColor" value="red" />
				<Color className="color" color="red"/>
			</label>
			<label>
				<input checked={`${props.playerColor == 'black' ? 'true' : ''}`} onChange={(event) => props.changePlayerColor(event.target.value)} type="radio" id="black" name="playerColor" value="black" />
				<Color className="color" color="black"/>
			</label>
		</div>
	);
}