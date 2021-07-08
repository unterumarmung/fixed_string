import React from 'react';
import clsx from 'clsx';
import Layout from '@theme/Layout';
import Link from '@docusaurus/Link';
import useDocusaurusContext from '@docusaurus/useDocusaurusContext';
import styles from './index.module.css';

function HomepageHeader() {
  const { siteConfig } = useDocusaurusContext();
  const introButtonStyles = {
    marginRight: '5px'
  }

  return (
    <header className={clsx('hero hero--primary', styles.heroBanner)}>
      <div className="container">
        <h1 className="hero__title">{siteConfig.title}</h1>
        <p className="hero__subtitle">{siteConfig.tagline}</p>
        <div className={styles.buttons}>
          <Link
            className="button button--secondary button--lg"
            to="/docs/about"
            style={introButtonStyles}>
            Introduction
          </Link>
          <Link
            className="button button--secondary button--lg"
            to="/docs/api/basic_fixed_string">
            API Reference
          </Link>
        </div>
      </div>
    </header>
  );
}

export default function Home() {
  return (
    <Layout
      description="Description will go into a meta tag in <head />">
      <main className="main">
        <HomepageHeader />
      </main>
    </Layout>
  );
}
