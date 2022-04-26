### What we have as a config:

- Choose the port and host of each ’server’.
- Setup the server_names or not.
- The first server for a host:port will be the default for this host:port (reply unbelonging request)
- Setup default error pages.
- Limit client body size.
- Setup routes with one or multiple of the following rules/configuration (no regexp):
    + Define a list of accepted HTTP methods for the route.
    + Define a HTTP redirection.
    + Define a directory or a file from where the file should be searched
        * example: if url /kapouet is rooted to /tmp/www, url /kapouet/pouic/toto is /tmp/www/pouic/toto).
- Turn on or off directory listing.
- Set a default file to answer if the request is a directory.
- Execute CGI based on certain file extension (for example .php).
- Make the route able to accept uploaded files and configure where they should be saved.


### Items types for what we have in the config :

- port: number `integer`
- host, server_names : word `string`
- error pages: {errors:`array`, location:`string`}
- body size limit : number `integer` with `char` like: `100M`
- directory listing: `location / {autoindex off|on }`

+ host:port examples
    * `listen 127.0.0.1:8000;`
    * `listen 127.0.0.1;`
    * `listen 8000;`
    * `listen *:8000;`
    * `listen localhost:8000;`

+ server_name example
    * `server_name youpi;`

+ error pages example
    * `error_page 404 403 500 503 /error-page.html;`

+ body size limit examples
    * `client_body_buffer_size 20;`
    * `client_body_buffer_size 20k;`
    * `client_body_buffer_size 20m;`

+ default file to answer examples
    * `index index.html;`
    * `index index.html index.php home.html;`

+ location example
    * ```
        location /put_test {
                allow_methods PUT POST
                root ./YoupiBanane/put_here
                return 301 $scheme://service-new.company.com/$request_uri
        }

### CGI
    ∗ Do you wonder what a CGI is?
    ∗ Because you won’t call the CGI directly, use the full path as PATH_INFO.
    ∗ Just remember that, for chunked request, your server needs to unchunked it and the CGI will expect EOF as end of the body.
    ∗ Same things for the output of the CGI. If no content_length is returned from the CGI, EOF will mark the end of the returned data.
    ∗ Your program should call the CGI with the file requested as first argument.
    ∗ The CGI should be run in the correct directory for relative path file access.
    ∗ Your server should work with one CGI (php-CGI, Python, and so forth).
