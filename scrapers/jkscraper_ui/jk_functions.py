import re as regex

def sanitizeHtml(html):
    return regex.sub(r"[\n\t\s]*", "", str(html).replace("\\n", "").replace("\\t", ""))