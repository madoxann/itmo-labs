#include "entrypoint.h"

#include "entities.h"
#include "http.h"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("author");
MODULE_VERSION("0.000001");

char token[37];

void get_encoded(char *str, char *result, int sz) {
  int j = 0;

  for (int i = 0; i < sz; i += 3) {
    result[i] = '%';
    sprintf(result + i + 1, "%02x", str[j++]);
  }
}

int handle_errno(int64_t no) {
  if (!no) return 0;
  if (no < 0)
    printk(KERN_ERR, http_messages[-no]);
  else
    printk(KERN_ERR, documentation_messages[no]);

  return 1;
}

struct dentry *networkfs_mount(struct file_system_type *fs_type, int flags,
                               const char *tkn, void *data) {
  struct dentry *ret;
  ret = mount_nodev(fs_type, flags, data, networkfs_fill_super);

  if (ret == NULL)
    printk(KERN_ERR "Can't mount file system");
  else
    printk(KERN_INFO "Mounted successfuly");

  for (int i = 0; i < 37; i++) token[i] = tkn[i];

  return ret;
}

int networkfs_fill_super(struct super_block *sb, void *data, int silent) {
  struct inode *inode;
  inode = networkfs_get_inode(sb, NULL, S_IFDIR, 1000);
  sb->s_root = d_make_root(inode);

  if (sb->s_root == NULL) return -ENOMEM;

  printk(KERN_INFO "return 0\n");
  return 0;
}

struct inode *networkfs_get_inode(struct super_block *sb,
                                  const struct inode *dir, umode_t mode,
                                  int i_ino) {
  struct inode *inode;
  inode = new_inode(sb);
  inode->i_fop = &networkfs_dir_ops;
  inode->i_op = &networkfs_inode_ops;
  inode->i_ino = i_ino;
  inode->i_mode |= 777;

  if (inode != NULL) {
    inode_init_owner(&init_user_ns, inode, dir, mode);
  }
  return inode;
};

int networkfs_iterate(struct file *filp, struct dir_context *ctx) {
  char fsname[10];
  struct dentry *dentry;
  struct inode *inode;
  unsigned long offset;
  int stored;
  unsigned char ftype;
  ino_t ino;
  ino_t dino;
  dentry = filp->f_path.dentry;
  inode = dentry->d_inode;
  offset = filp->f_pos;
  stored = 0;
  ino = inode->i_ino;
  struct entries response;
  char my_ino[11];
  snprintf(my_ino, 11, "%d", ino);
  if (handle_errno(networkfs_http_call(token, "list", (void *)&response,
                                       sizeof(struct entries), 1, "inode",
                                       my_ino))) {
    return -1;
  }

  if (offset >= response.entries_count) {
    return 0;
  }

  strcpy(fsname, ".");
  ftype = DT_DIR;
  dino = ino;
  ctx->pos++;
  dir_emit(ctx, ".", 1, dino, ftype);

  strcpy(fsname, "..");
  dino = dentry->d_parent->d_inode->i_ino;
  ctx->pos++;
  dir_emit(ctx, "..", 2, dino, ftype);

  for (int i = 0; i < response.entries_count; i++) {
    printk("%s\n", response.entries[i].name);
    dir_emit(ctx, response.entries[i].name, strlen(response.entries[i].name),
             response.entries[i].ino, response.entries[i].entry_type);
  }
  ctx->pos += response.entries_count;

  return response.entries_count;
}

struct dentry *networkfs_lookup(struct inode *parent_inode,
                                struct dentry *child_dentry,
                                unsigned int flag) {
  ino_t root;
  struct inode *inode;
  const char *name = child_dentry->d_name.name;
  root = parent_inode->i_ino;
  struct entry_info http_ans;
  char my_ino[11];
  snprintf(my_ino, 11, "%d", root);

  int length = strlen(name) * 3 + 1;
  char enc_str[length];
  get_encoded(name, enc_str, length - 1);

  if (handle_errno(networkfs_http_call(token, "lookup", (void *)&http_ans,
                                       sizeof(struct entry_info), 2, "parent",
                                       my_ino, "name", enc_str))) {
    return NULL;
  }

  inode = networkfs_get_inode(parent_inode->i_sb, NULL,
                              http_ans.entry_type == DT_DIR ? S_IFDIR : S_IFREG,
                              http_ans.ino);
  d_add(child_dentry, inode);
  return NULL;
}

int networkfs_new(struct user_namespace *ns, struct inode *parent_inode,
                  struct dentry *child_dentry, umode_t mode, int is_file) {
  ino_t root;
  struct inode *inode;
  const char *name = child_dentry->d_name.name;
  root = parent_inode->i_ino;
  ino_t http_ans = 0;
  char my_ino[11];
  snprintf(my_ino, 11, "%d", root);

  int length = strlen(name) * 3 + 1;
  char enc_str[length];
  get_encoded(name, enc_str, length - 1);

  if (handle_errno(networkfs_http_call(
          token, "create", (ino_t)&http_ans, sizeof(ino_t), 3, "parent", my_ino,
          "name", enc_str, "type", is_file ? "file" : "directory"))) {
    return -1;
  }
  inode =
      networkfs_get_inode(parent_inode->i_sb, NULL,
                          (is_file ? S_IFREG : S_IFDIR) | S_IRWXUGO, http_ans);
  inode->i_op = &networkfs_inode_ops;
  inode->i_fop = NULL;
  d_add(child_dentry, inode);

  return 0;
}

int networkfs_rm(struct inode *parent_inode, struct dentry *child_dentry,
                 int is_file) {
  const char *name = child_dentry->d_name.name;
  ino_t root;
  root = parent_inode->i_ino;
  ino_t http_ans = 0;
  char my_ino[11];
  snprintf(my_ino, 11, "%d", root);

  int length = strlen(name) * 3 + 1;
  char enc_str[length];
  get_encoded(name, enc_str, length - 1);

  if (handle_errno(networkfs_http_call(token, (is_file ? "unlink" : "rmdir"),
                                       (ino_t)&http_ans, sizeof(ino_t), 2,
                                       "parent", my_ino, "name", enc_str))) {
    return -1;
  }
  return 0;
}

int networkfs_create(struct user_namespace *ns, struct inode *parent_inode,
                     struct dentry *child_dentry, umode_t mode, bool b) {
  return networkfs_new(ns, parent_inode, child_dentry, mode, 1);
}

int networkfs_create_directory(struct user_namespace *ns,
                               struct inode *parent_inode,
                               struct dentry *child_dentry, umode_t mode) {
  return networkfs_new(ns, parent_inode, child_dentry, mode, 0);
}

int networkfs_unlink(struct inode *parent_inode, struct dentry *child_dentry) {
  return networkfs_rm(parent_inode, child_dentry, 1);
}

int networkfs_unlink_rmdir(struct inode *parent_inode,
                           struct dentry *child_dentry) {
  return networkfs_rm(parent_inode, child_dentry, 0);
}

int networkfs_link(struct dentry *old_dentry, struct inode *parent_dir,
                   struct dentry *new_dentry) {
  int errcode = 0;
  const char parent_ino[11], source_ino[11];

  snprintf(parent_ino, 11, "%d", parent_dir->i_ino);
  snprintf(source_ino, 11, "%d", old_dentry->d_inode->i_ino);
  return networkfs_http_call(token, "link", (int)&errcode, sizeof(int), 3,
                             "source", source_ino, "parent", parent_ino, "name",
                             new_dentry->d_name.name);
}

void networkfs_kill_sb(struct super_block *sb) {
  printk(KERN_INFO
         "networkfs super block is destroyed. Unmount successfully.\n");
}

int networkfs_init(void) {
  if (register_filesystem(&networkfs_fs_type)) {
    printk(KERN_INFO "Error networkfs init!\n");
    return 1;
  }
  printk(KERN_INFO "Hello, World!\n");
  return 0;
}

void networkfs_exit(void) {
  unregister_filesystem(&networkfs_fs_type);
  printk(KERN_INFO "Goodbye!\n");
}

module_init(networkfs_init);
module_exit(networkfs_exit);