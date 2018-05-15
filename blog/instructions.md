## upload instructions
* aws-shell
  s3 sync ~/entegrity.blog/blog/output s3://entegrity.co/blog

## build

* cd blog
* pelican content
* run server locally python -m pelican.server (ensure it run from output dir) 

* from weblog branch ghp-import output -r origin -b master
generate output to master branch
git push origin master
