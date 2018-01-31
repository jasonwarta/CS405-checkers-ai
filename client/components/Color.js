import React from 'react';

export default function Color(props) {
	return (
		<div style={{backgroundColor: props.color}} {...props} ></div>
	);
}