const lightCodeTheme = require('prism-react-renderer/themes/github');
const darkCodeTheme = require('prism-react-renderer/themes/dracula');

/** @type {import('@docusaurus/types').DocusaurusConfig} */
module.exports = {
  title: 'fixed_string',
  tagline: 'C++17 fixed-size string class',
  url: 'https://unterumarmung.github.io/fixed_string/',
  baseUrl: '/fixed_string/',
  onBrokenLinks: 'throw',
  onBrokenMarkdownLinks: 'warn',
  favicon: 'img/favicon.ico',
  organizationName: 'unterumarmung',
  projectName: 'fixed_string',
  themeConfig: {
    navbar: {
      title: 'fixed_string',
      logo: {
        alt: 'fixed_string logo',
        src: 'img/logo.svg',
      },
      items: [
        {
          to: 'docs/about',
          position: 'left',
          label: 'Documentation',
        },
        {
          href: 'https://github.com/unterumarmung/fixed_string',
          label: 'GitHub',
          position: 'right',
        },
      ],
    },
    footer: {
      style: 'dark',
      copyright: `Copyright © ${new Date().getFullYear()} Daniil Dudkin. Built with Docusaurus.`,
    },
    prism: {
      theme: lightCodeTheme,
      darkTheme: darkCodeTheme,
    },
  },
  presets: [
    [
      '@docusaurus/preset-classic',
      {
        docs: {
          sidebarPath: require.resolve('./sidebars.js'),
          // Please change this to your repo.
          editUrl:
            'https://github.com/unterumarmung/fixed_string/edit/add-documentation/documentation/',
        },
        theme: {
          customCss: require.resolve('./src/css/custom.css'),
        },
      },
    ],
  ],
};
