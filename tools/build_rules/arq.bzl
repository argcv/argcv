def _is_windows(ctx):
    return ctx.os.name.lower().find("windows") != -1

def _wrap_bash_cmd(ctx, cmd):
    if _is_windows(ctx):
        bazel_sh = _get_env_var(ctx, "BAZEL_SH")
        if not bazel_sh:
            fail("BAZEL_SH environment variable is not set")
        cmd = [bazel_sh, "-l", "-c", " ".join(cmd)]
    return cmd

def _get_env_var(ctx, name):
    if name in ctx.os.environ:
        return ctx.os.environ[name]
    else:
        return None

# Executes specified command with arguments and calls 'fail' if it exited with
# non-zero code
def _execute_and_check_ret_code(repo_ctx, cmd_and_args):
    result = repo_ctx.execute(cmd_and_args, timeout = 10)
    if result.return_code != 0:
        fail(("Non-zero return code({1}) when executing '{0}':\n" + "Stdout: {2}\n" +
              "Stderr: {3}").format(
            " ".join(cmd_and_args),
            result.return_code,
            result.stdout,
            result.stderr,
        ))

def _repos_are_siblings():
    return Label("@foo//bar").workspace_root.startswith("../")

# Apply a patch_file to the repository root directory
# Runs 'patch -p1'
def _apply_patch(ctx, patch_file):
    # Don't check patch on Windows, because patch is only available under bash.
    if not _is_windows(ctx) and not ctx.which("patch"):
        fail("patch command is not found, please install it")
    cmd = _wrap_bash_cmd(
        ctx,
        ["patch", "-p1", "-d", ctx.path("."), "-i", ctx.path(patch_file)],
    )
    _execute_and_check_ret_code(ctx, cmd)

def _apply_delete(ctx, paths):
    for path in paths:
        if path.startswith("/"):
            fail("refusing to rm -rf path starting with '/': " + path)
        if ".." in path:
            fail("refusing to rm -rf path containing '..': " + path)
    cmd = _wrap_bash_cmd(ctx, ["rm", "-rf"] + [ctx.path(path) for path in paths])
    _execute_and_check_ret_code(ctx, cmd)

# for arq
# Temporary workaround to support including current project as a submodule until this
# use-case is supported in the next Bazel release.
def _arq_impl(ctx):
    urls = ctx.attr.urls
    output = ""
    sha256 = ctx.attr.sha256
    type = ctx.attr.type
    strip_prefix = ctx.attr.strip_prefix
    ctx.download_and_extract(
        urls,  # url
        output,  # output
        sha256,  # sha256
        type,  # type (e.gi. zip, jar, war, tar.gz, tgz, tar.bz2, tar.xz )
        strip_prefix,
    )  # stripPrefix

    # remove specified files
    if ctx.attr.delete:
        _apply_delete(ctx, ctx.attr.delete)

    #
    if ctx.attr.patch_file != None:
        _apply_patch(ctx, ctx.attr.patch_file)

    # apply template file
    if ctx.attr.build_file != None:
        ctx.template(
            "BUILD",  # path
            ctx.attr.build_file,  # template
            {
                "%prefix%": ".." if _repos_are_siblings() else "external",
                "%ws%": ctx.attr.repository,
            },  # substitutions
            False,
        )  # executable

# TODO(yu): add local search path
arq_configure = repository_rule(
    attrs = {
        "urls": attr.string_list(default = []),
        "build_file": attr.label(),
        "patch_file": attr.label(),
        "sha256": attr.string(mandatory = True),
        "type": attr.string(),
        "strip_prefix": attr.string(default = ""),
        "repository": attr.string(mandatory = True),
        "delete": attr.string_list(),
    },
    implementation = _arq_impl,
)

def arq(name, **kwargs):
    if name in native.existing_rules():
        # print("Archive [", name, "] already exists, skipping...")
        return
    print("[ARQ] **", name, "** fetching...")
    arq_configure(
        name = name,
        **kwargs
    )
    print("[ARQ] **", name, "** fetched")

def clean_dep(dep):
    return str(Label(dep))
