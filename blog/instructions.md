## upload instructions
* aws-shell
  s3 sync ~/entegrity.blog/blog/output s3://entegrity.co/blog
## generate css styles
```
pygmentize -S monokai -f html -a .codehilite > code.css
```
## build

* cd blog
* make html
* make serve or
 > run server locally python -m pelican.server (ensure it run from output dir) 

* from weblog branch ghp-import output -r origin -b master
generate output to master branch
git push origin master
