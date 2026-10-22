# std.html — HTML5 parser and toolkit

```tauraro
from std.html import html_parse, html_to_text, html_links, html_sanitize_default
from std.html import HtmlDocument, HtmlNode, HtmlSelector, HtmlPolicy
from std.html import html_escape, html_unescape
```

A forgiving HTML5 parser following the WHATWG tokenization and tree-construction
algorithms, a small DOM, CSS selectors, and a few utilities. Parsing never
fails: every input produces a tree, exactly as a browser would build it
(`<p>a<p>b`, misnested `<b><i></b></i>`, tables with stray text, unclosed
tags, missing `<html>/<head>/<body>`, ...).

Modules: `std.html` (re-exports everything below), `std.html.entities`,
`std.html.tokenizer`, `std.html.dom`, `std.html.util`. The named character
reference table is a C file (`std/html/html_native.c`, linked automatically).

```tauraro
mut doc = html_parse("<ul id=nav><li><a href='/a'>A<li><a href='b.html?x=1&amp;y=2'>B</ul>")
for a in doc.select("#nav li > a"):
    print(a.attr("href") + " -> " + a.text())
mut links = html_links(doc, "https://example.com/dir/")
print(links[1].url)                    # https://example.com/dir/b.html?x=1&y=2
print(html_sanitize_default("<p onclick=x>hi<script>bad()</script></p>"))   # <p>hi</p>
```

## Parsing

| Function | Description |
|---|---|
| `html_parse(src: str) -> HtmlDocument` | Parse a complete document (also `HtmlDocument.parse`). |
| `html_parse_fragment(src: str) -> HtmlNode` | Parse markup in a `<body>` context; returns a container element, use `.inner_html()` / `.children()`. |

## HtmlDocument

| Member | Description |
|---|---|
| `document()`, `root()`, `head()`, `body()` | Document node, `<html>`, `<head>`, `<body>` (or `<frameset>`). |
| `doctype()` | The doctype node or a null node. `quirks` field: quirks mode flag. |
| `title()` | Whitespace-collapsed text of the first `<title>`. |
| `select(css)`, `select_one(css)` | Selector queries over the whole document. |
| `to_html()` | Serialize the document. |
| `create_element(tag)`, `create_text(s)`, `create_comment(s)` | Make detached nodes (attach with `append_child` / `insert_before`). |
| `debug_tree()` | The tree in the html5lib-tests text format. |

## HtmlNode

A node is a small handle `{doc, id}`; the document owns all nodes, so there are
no reference cycles. Compare nodes with `a.same(b)`, not `==`. Misses return a
**null node** (`is_null()`); every accessor is safe on it (empty string, empty
list, further null nodes).

| Group | Members |
|---|---|
| Kind | `kind()` (`HTML_ELEMENT`, `HTML_TEXT`, `HTML_COMMENT`, `HTML_DOCUMENT`, `HTML_DOCTYPE`), `is_element()`, `is_text()`, `is_comment()`, `is_doctype()`, `is_document()`, `is_null()` |
| Name / data | `tag()` (lowercase; SVG keeps camelCase), `namespace()` (`html`/`svg`/`math`), `data()` (text/comment/doctype name), `doctype_public_id()`, `doctype_system_id()` |
| Attributes | `attr(name)` (`""` when absent), `has_attr`, `attrs() -> List[HtmlAttr]` (`.name`, `.value`), `attr_names()`, `set_attr`, `remove_attr`, `element_id()`, `classes()`, `has_class(name)` |
| Navigation | `parent()`, `first_child()`, `last_child()`, `next_sibling()`, `prev_sibling()`, `next_element()`, `prev_element()`, `children()`, `child_elements()`, `descendants()`, `find(tag)`, `find_all(tag)`, `closest(css)` |
| Content | `text()` (DOM textContent), `normalized_text()` (whitespace collapsed and trimmed), `text_ws(normalize)`, `inner_html()`, `outer_html()` |
| Mutation | `append_child`, `insert_before(child, ref)`, `remove()`, `set_text(s)`, `set_inner_html(markup)`, `set_data(s)` |
| Selectors | `select(css)`, `select_one(css)`, `matches(css)` |

Serialization follows the WHATWG fragment serializer: text escapes `& < >` and
U+00A0, attribute values use double quotes and escape `& "` (and `< >`),
void elements have no end tag, `<script>/<style>/<xmp>/<iframe>/<noembed>/<noframes>/<plaintext>`
text is raw, and the newline swallowed after `<pre>/<textarea>/<listing>` is
restored. `html_parse(doc.to_html())` yields the same tree.

## CSS selectors

Supported: type, `*`, `#id`, `.class`, `[a]`, `[a=v]`, `[a~=v]`, `[a|=v]`,
`[a^=v]`, `[a$=v]`, `[a*=v]` (quoted or bare values, optional ` i` flag),
descendant, `>`, `+`, `~`, selector lists, `:first-child`, `:last-child`,
`:only-child`, `:nth-child(an+b | odd | even)`, `:nth-last-child()`, the
`-of-type` variants, `:not(list)`, `:is()`, `:where()`, `:has()` (descendant
and `>`/`+`/`~` relative arguments), `:empty`, `:root`, `:checked`,
`:disabled`, `:enabled`, and CSS escapes in identifiers. Type and attribute
names match case-insensitively; ids, classes and values are case-sensitive.

`select` returns descendants of the node in document order (not the node
itself). Invalid selectors match nothing; use `HtmlSelector.parse(css)`
(`throws str`, with a position in the message) or
`HtmlSelector.parse_lenient(css)` (`.ok`, `.error`) to check. A compiled
`HtmlSelector` can be reused: `sel.select(node)`, `sel.select_one(node)`,
`sel.matches(node)`.

## Escaping

| Function | Description |
|---|---|
| `html_escape(s)` | `& < > " '` to `&amp; &lt; &gt; &quot; &#x27;` |
| `html_unescape(s)` | Decode all references the way the tokenizer does: the full table of 2231 names, legacy names without `;` (longest match, `&notit;` is `¬it;`), decimal/hex numerics, and the fix-ups (0, surrogates, > U+10FFFF to U+FFFD; 0x80-0x9F via Windows-1252). |
| `html_unescape_attr(s)` | Same with the attribute rule (a legacy name without `;` followed by `=` or an alphanumeric stays literal). |
| `html_entity(name) -> List[int]` | Code points of a name (`"hearts"`, `"&amp;"`); empty if unknown. |

## Text, links, sanitizer

| Function | Description |
|---|---|
| `html_to_text(node) -> str` | Readable text: scripts/styles/head/comments skipped, whitespace collapsed (verbatim in `<pre>`), blocks on new lines, blank line around paragraphs/headings/lists/tables, `<br>` newline, `- ` / `1. ` list markers (`<ol start>` honoured), tabs between table cells, `<img alt>`. |
| `html_strip_tags(markup) -> str` | `html_to_text` of a string (document or fragment). |
| `html_links(doc, base_url) -> List[HtmlLink]` | `<a href>` and `<area href>` in order; fields `url` (resolved with `std.net.url`, honouring `<base href>`; `mailto:` etc. returned as written), `href`, `text`, `rel`, `node`. |
| `html_sanitize(markup, policy) -> str` | Parse, filter and re-serialize. Elements not on the allow-list are unwrapped (children kept); the policy's drop list (script, style, iframe, object, embed, svg, math, form controls, ...) is removed with content; attributes not allowed for the tag, `on*` handlers and URL attributes with a disallowed scheme are removed (leading control characters, tabs/newlines inside the scheme and entity-encoded schemes are handled because filtering works on the parsed tree). `data:` URLs pass only on `<img src>` with image types when `allow_data_images` is set. Output is well-formed. |
| `html_sanitize_default(markup)` | `html_sanitize` with `HtmlPolicy.default()`. |

`HtmlPolicy.default()` is a safe policy for user content (formatting, lists,
tables, links, images; `http https mailto` and relative URLs; no `id`, `style`,
forms or scripts). `HtmlPolicy.basic()` is inline formatting and links only,
`HtmlPolicy.empty()` allows nothing (text only). Builders: `allow_tags("a, em")`,
`deny_tags`, `drop_tags`, `allow_attrs(tag_or_star, "href title")`,
`allow_schemes("ftp")`; fields `allow_data_images`, `allow_relative`,
`keep_comments`, `link_rel` (forces e.g. `rel="nofollow noopener"` on links).

## Conformance and limitations

The tree builder was checked against html5lib 1.1 on 5000 generated inputs
(hand-written tricky cases plus random tag soup including tables, selects,
formatting elements, SVG/MathML, frameset, raw text): 99.7% identical. The
remaining differences are places where html5lib trails the current spec or
differs from it (`<rt>` after `<rb>` closes the `<rb>` here, per the spec;
html5lib nests them), the "skip the newline after `<pre>`" flag, which html5lib
keeps across ignored tokens, and a few frameset/whitespace and `<dd>` in
foreign content corner cases.

- `<template>` is an ordinary element (no separate content fragment); scripting
  is treated as disabled, so `<noscript>` content is parsed as markup.
- Input is UTF-8; there is no encoding sniffing and no `<meta charset>` handling.
- `select` is a straightforward matcher (no indexes); sibling-position
  pseudo-classes cost O(siblings). Fine for page-sized documents.
- The fragment parser always uses a `<body>` context.
- Entities in `html_unescape` follow browsers, not Python's `html.unescape`
  (which drops some control code points).
