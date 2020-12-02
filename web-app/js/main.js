export let escapeHtml = (html) => {
  html.replace('&', '&amp;')
  html.replace('<', '&lt;')
  html.replace('>', '&gt;')
  html.replace('"', '&quot;')
  return html
}
