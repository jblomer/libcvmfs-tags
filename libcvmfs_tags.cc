/**
 * Access a specific tag of a cvmfs repository.
 */

#include <libcvmfs.h>
#include <unistd.h>

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>


const char *repository_name = "lib.cern.ch";
const char *tag = "v1";
const char *cache_dir = "/tmp/libcvmfs";
const char *stratum1 = "http://localhost/cvmfs/lib.cern.ch";
const char *proxies = "DIRECT;";
const char *public_key =
"-----BEGIN PUBLIC KEY-----\n"
"MIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEAsNsId6dFKGavdAEgnE7o\n"
"2juPfKrvK6OEj31gwkBqdc0ESCENalC2tLGin3tPycMychzSpgNoBE+82i7dJXuo\n"
"QKbBOjN3RedKLtM6PuqvORs7bRPRYJh+J7qbiZBANBDPMW1r9kq2hm+FyUJxZHa2\n"
"C0YahjjN8v/OX8ZnlW+ovVCTubX5PiMt8DHkjgkK20s3mkvZd9NSic6BG2UlVjoX\n"
"j4t5a59heMHkS1lGMSfKCcR/fvPDzzViXJDM2R+RPkzcvlNdgv5YP8ks53Q9zuUw\n"
"NROmqRBD+8klmnMmXUh6D1i6CllHbfIuk3NMjPaDat/zlFcic8e6T56eH8LbCrso\n"
"qwIDAQAB\n"
"-----END PUBLIC KEY-----\n";


static cvmfs_context *Mount() {
  // Global cvmfs initialization
  std::string global_options = std::string("cache_directory=") + cache_dir;
  int retval = cvmfs_init(global_options.c_str());
  if (retval != 0) {
    fprintf(stderr, "couldn't initialize libcvmfs!\n");
    return NULL;
  }

  // Write public key to a temporary file
  char *public_key_file = strdupa("/tmp/libcvmfs.pubkey.XXXXXX");
  int tmp_fd = mkstemp(public_key_file);
  if (tmp_fd < 0) {
    fprintf(stderr, "couldn't dump public key!\n");
    return NULL;
  }
  write(tmp_fd, public_key, strlen(public_key));
  close(tmp_fd);

  // Attache repository
  cvmfs_context *ctx = NULL;
  std::string repo_options = std::string("repo_name=") + repository_name +
    ",url=" + stratum1 + ",proxies=" + proxies + ",pubkey=" + public_key_file;
  ctx = cvmfs_attach_repo(repo_options.c_str());
  unlink(public_key_file);
  if (ctx == NULL)
    fprintf(stderr, "couldn't initialize repository!\n");
  return ctx;
}

static void Unmount(cvmfs_context *ctx) {
  cvmfs_detach_repo(ctx);
  cvmfs_fini();
}


int main(int argc, char *argv[]) {
  cvmfs_context *ctx = Mount();
  if (ctx == NULL)
    return 1;

  Unmount(ctx);
  return 0;
}

