import React from 'react';
import CodeBlock from '@theme/CodeBlock';

export const Highlight = ({ children, color }) => (
    <span
        style={{
            backgroundColor: color,
            borderRadius: '2px',
            color: '#fff',
            padding: '0.2rem',
        }}>
        {children}
    </span>
);

const cppBlockStyles = {
    marginBottom: '0px !important'
}

export default function CppOverload({ num, code, standard }) {
    return <tr>
        <td><h6>{num}</h6></td>
        <td><CodeBlock styles={cppBlockStyles} className="language-cpp">{code}</CodeBlock></td>
        <td>{standard && <Highlight color="#00a550">{"since C++" + standard}</Highlight>}</td>
    </tr>
}


