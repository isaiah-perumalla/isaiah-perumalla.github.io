A minimalist theme for the Pelican static blog generator:

  http://blog.getpelican.com/

It's based on the built-in simple theme with some small changes to the
templates and some very minimalist CSS.

Install like this:

  # pelican-themes -i mc-pelican

Then, in your pelicanconf.py:

  THEME = 'mc-pelican'
  
You add menu items with MENUITEMS like this:

MENUITEMS = (
    ('Homepage', '/mc/'),
    ('About MC', '/mc/bio.html'),
    ('Contact', '/mc/contact.html'),
    ('Archives', '{0}/archives.html'.format(SITEURL)), )

If you want a blog description in the footer of all pages, define
DESCRIPTION.

Happy blogging,
MC.
