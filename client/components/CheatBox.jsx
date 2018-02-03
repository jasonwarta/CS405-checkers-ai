import React from 'react';

export default function CheatBox(props) {
	let id = props.label.split(' ').join('-').toLowerCase();
	return (
		<div>
			<label>{props.label}
				<input id={id} type="text"/>
			</label>
			<button onClick={() => {props.submitMove(document.getElementById(id).value)}}>Enter</button>
		</div>
	);
}