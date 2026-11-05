import { defineConfig } from "vitepress";

export default defineConfig({
  title: "DDL",
  description:
    "A lightweight C++17 library for storing and working with dynamic data.",
  base: "/ddl/",

  themeConfig: {
    nav: [
      { text: "Home", link: "/" },
      { text: "Guide", link: "/guide/getting-started" },
      { text: "API Reference", link: "/api/" },
    ],

    sidebar: {
      "/guide/": [
        {
          text: "Introduction",
          items: [
            { text: "Getting Started", link: "/guide/getting-started" },
            { text: "Quick Start", link: "/guide/quick-start" },
          ],
        },
        {
          text: "Core Concepts",
          items: [
            { text: "Values and Types", link: "/guide/values-and-types" },
            { text: "Type Conversions", link: "/guide/type-conversions" },
            { text: "Arrays and Objects", link: "/guide/arrays-and-objects" },
          ],
        },
      ],

      "/api/": [
        {
          text: "API Reference",
          items: [
            { text: "Overview", link: "/api/" },
            { text: "Value Class", link: "/api/value" },
            { text: "Stream Insertion Operator", link: "/api/stream-operator" },
          ],
        },
      ],
    },

    socialLinks: [
      { icon: "github", link: "https://github.com/amanjkhan21/ddl" },
    ],

    footer: {
      message: "Released under the MIT License.",
      copyright: "Copyright © 2026 DDL Contributors",
    },
  },
});
