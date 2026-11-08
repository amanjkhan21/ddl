import { defineConfig } from "vitepress";

export default defineConfig({
  title: "DDL",
  description: "Documentation for DDL — Dynamic Data Library for C++.",
  base: "/ddl/",

  themeConfig: {
    siteTitle: "DDL",

    nav: [
      {
        text: "Guide",
        link: "/guide/introduction",
      },
    ],

    search: {
      provider: "local",
    },

    socialLinks: [
      { icon: "github", link: "https://github.com/amanjkhan21/ddl" },
    ],

    sidebar: {
      "/guide/": [
        {
          text: "Guide",
          items: [
            { text: "Introduction", link: "/guide/introduction" },
            { text: "Installation", link: "/guide/installation" },
            { text: "Quick Start", link: "/guide/quick-start" },
            { text: "Supported Types", link: "/guide/types" },
            { text: "Type Checking", link: "/guide/type-checking" },
            { text: "Conversions", link: "/guide/conversions" },
            { text: "References", link: "/guide/references" },
            { text: "Arrays and Objects", link: "/guide/collections" },
            { text: "Printing", link: "/guide/printing" },
            { text: "Exceptions", link: "/guide/exceptions" },
          ],
        },
      ],
    },

    footer: {
      message: "DDL — Dynamic Data Library for C++",
      copyright: "Copyright © Aman Khan",
    },
  },
});
