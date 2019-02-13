#!/usr/bin/env python
# -*- coding: utf-8 -*- #
from __future__ import unicode_literals

AUTHOR = 'isaiah_p'
SITENAME = 'Isaiah Perumalla'
SITEURL = ''

PATH = 'content'
STATIC_PATHS = ['imgs', 'downloads']

THEME = 'mc-pelican'
TIMEZONE = 'Europe/London'
GOOGLE_ANALYTICS_ID = 'UA-109572315-1'

DEFAULT_LANG = 'en'

# Feed generation is usually not desired when developing
FEED_ALL_ATOM = None
CATEGORY_FEED_ATOM = None
TRANSLATION_FEED_ATOM = None
AUTHOR_FEED_ATOM = None
AUTHOR_FEED_RSS = None

MENUITEMS = (
    ('About', '/pages/about.html'),
    ('Archives', '{0}/archives.html'.format(SITEURL)), )
PLUGINS = [
    'pelican_gist',
]
# Blogroll
LINKS = (('Pelican', 'http://getpelican.com/'),
         ('Python.org', 'http://python.org/'),
         ('Jinja2', 'http://jinja.pocoo.org/'),
         ('You can modify those links in your config file', '#'),)

# Social widget
SOCIAL = (('You can add links in your config file', '#'),
          ('Another social link', '#'),)

DEFAULT_PAGINATION = 40

# Uncomment following line if you want document-relative URLs when developing
#RELATIVE_URLS = True
