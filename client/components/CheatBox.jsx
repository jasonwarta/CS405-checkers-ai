import React from 'react';

export default function CheatBox(props) {
	return (
		<div>
			<label>Board String 
				<input id="cheatBox" type="text"/>
			</label>
			<button onClick={() => {props.submitMove(document.getElementById('cheatBox').value)}}>Send</button>
		</div>
	);
}