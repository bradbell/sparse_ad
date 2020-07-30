# For conf.py documentation see
# http://www.sphinx-doc.org/en/master/config
#
# -- Path setup --------------------------------------------------------------
import sphinx_rtd_theme

# -- Project information -----------------------------------------------------
project   = 'sparse_ad'
copyright = '2020'
author    = 'Brad Bell'
version   = '20200729'

# -- General configuration ---------------------------------------------------
extensions = [
    'sphinx.ext.mathjax',
    'sphinx_rtd_theme',
]
exclude_patterns = [
    '_build', 'Thumbs.db', '.DS_Store', 'test_out', 'preamble.rst'
]

# -- Options for HTML output -------------------------------------------------
html_theme = 'sphinx_rtd_theme'
html_theme_options = {
    'navigation_depth' : -1   ,
    'titles_only'      : True ,
}
# END_CONF_PY
