import React from 'react';

export default function MoveTracker(props) {
	return (
		<div className="moveTracker">
			<textarea cols="50" rows="50" 
				value={props.computerMoves.join('\n')}
				readOnly="true" />
		</div>
	);
}