BLOG_HOME=/home/isaiahp/projects/entegrity-blog
echo "reseting theme mc-pelican "
$BLOG_HOME/blog_env/bin/pelican-themes -r mc-pelican
echo "reinstalling theme mc-pelican "
$BLOG_HOME/blog_env/bin/pelican-themes -i $BLOG_HOME/themes/mc-pelican
echo "regen content "
$BLOG_HOME/blog_env/bin/pelican --relative-urls ~/projects/entegrity-blog/blog/content/
