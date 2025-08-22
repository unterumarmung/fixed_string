import React from 'react';
import Link from '@docusaurus/Link';

const buttonStyle = {
    display: 'flex',
    alignItems: 'center',
    justifyContent: 'flex-end'
}

export default function LinkButton({ link, children }) {
    return <div style={buttonStyle}>
        <Link
            className="button button--secondary button--lg"
            to={link}>
            {children}
        </Link>
    </div>
}