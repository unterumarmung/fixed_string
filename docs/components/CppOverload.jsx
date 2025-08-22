import React from 'react';
import CodeBlock from '@theme/CodeBlock';
import Heading from '@theme/Heading';

export const Highlight = ({ children, color }) => (
    <span
        style={{
            backgroundColor: color,
            borderRadius: '5px',
            color: '#fff',
            padding: '0.3rem',
        }}>
        {children}
    </span>
);

const tableRowStyles = {
    borderTop: '1px !important',
    borderBottom: '1px !important',
    backgroundColor: 'inherit'
}

const tableCellStyles = {
    borderRight: '0px',
    borderLeft: '0px',
    borderBottom: '1px !important',
    verticalAlign: 'center',
}

const codeBlockStyle = {
    borderBottom: '0px !important'
}

const numberStyle = {
    marginBottom: 0
}

const NumberComponent = Heading('h6');

export default function CppOverload({ num, code, standard }) {
    return <tr style={tableRowStyles}>
        <td style={tableCellStyles}><NumberComponent id={'overload' + num} style={numberStyle}>{num}</NumberComponent></td>
        <td className="cpp_overload_code" style={tableCellStyles}><CodeBlock style={codeBlockStyle} className="language-cpp">{code}</CodeBlock></td>
        <td style={tableCellStyles}>{standard && <Highlight color="#00a550">{"since C++" + standard}</Highlight>}</td>
    </tr>
}


