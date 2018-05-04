## upload instructions
* aws-shell
  s3 sync ~/entegrity.blog/blog/output s3://entegrity.co/blog

## build

* cd blog
* pelican content
* run server locally python -m pelican.server (ensure it run from output dir) 


## upload to google storage
* gcloud auth login #login using account
* from output dir, run gsutil rsync -R ./ gs://isaiahp.net
* allow read permission to all users, gsutil acl ch -u AllUsers:R gs://isaiahp.net
* gsutil web set -m index.html -e 404.html gs://isaiahp.net