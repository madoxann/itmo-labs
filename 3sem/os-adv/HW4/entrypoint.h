#include <linux/fs.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>

#define ENOINODE 1
#define ENOTAFILE 2
#define ENOTADIR 3
#define ERECORDNOTFOUND 4
#define EHASRECORD 5
#define EBEYONDSIZELIMIT 6
#define EBEYONDRECORDLIMIT 7
#define ENOTEMPTY 8
#define EBEYONDNAMELIMIT 9

static char *documentation_messages[] = {
    [ENOINODE] "No object found by inode number",
    [ENOTAFILE] "Not a file",
    [ENOTADIR] "Not a directory",
    [ERECORDNOTFOUND] "No record with such a name in this directory",
    [EHASRECORD] "There's already record with this name",
    [EBEYONDSIZELIMIT] "File too big",
    [EBEYONDRECORDLIMIT] "Too much files in directory",
    [ENOTEMPTY] "Directory is not empty",
    [EBEYONDNAMELIMIT] "Name too big"};

struct dentry *networkfs_mount(struct file_system_type *, int, const char *,
                               void *);
void networkfs_kill_sb(struct super_block *);
struct inode *networkfs_get_inode(struct super_block *, const struct inode *,
                                  umode_t, int);
int networkfs_fill_super(struct super_block *, void *, int);
int networkfs_iterate(struct file *filp, struct dir_context *ctx);
struct dentry *networkfs_lookup(struct inode *parent_inode,
                                struct dentry *child_dentry, unsigned int flag);
int networkfs_unlink(struct inode *, struct dentry *);
int networkfs_create(struct user_namespace *, struct inode *, struct dentry *,
                     umode_t, bool);
int networkfs_create_directory(struct user_namespace *, struct inode *,
                               struct dentry *, umode_t);
int networkfs_unlink_rmdir(struct inode *, struct dentry *);
int networkfs_link(struct dentry *, struct inode *, struct dentry *);

struct file_system_type networkfs_fs_type = {.name = "networkfs",
                                             .mount = networkfs_mount,
                                             .kill_sb = networkfs_kill_sb};

struct inode_operations networkfs_inode_ops = {
    .lookup = networkfs_lookup,
    .create = networkfs_create,
    .unlink = networkfs_unlink,
    .rmdir = networkfs_unlink_rmdir,
    .mkdir = networkfs_create_directory,
    .link = networkfs_link,
};

struct file_operations networkfs_dir_ops = {
    .iterate = networkfs_iterate,
};
