

Let me try to answer your earlier questions about what mirroring does and why mirroring and filtering have to go together. Git is less of a tool and more a data structure, so you need to shift your mental model a bit.

A quick refresher on git internals: Git maintains an immutable graph of commits, each of which maintains a _ref_ (technically a commit hash, intuitively a pointer) to its parent commits. Branches and tags are also just refs. There is a convention that the pointer on branches moves and the pointer on tags stays still. Git is garbage-collected, which means that (1) you never have a dangling ref, (2) if all pointers to a commit go away, eventually the commit will disappear, and (3) if there is are refs you don't know about, the commit definitely _won't_ be freed, like we are seeing here.

Git filter-branch is traversing every single commit, running an arbitrary command ('git rm ...' in this case), and creating a new commit whose data and metadata correspond exactly to the old, except (1) it has a different commit hash and (2) the results of that command have been applied.

Creating new commits is not enough, git-filter-branch also needs to update all refs to point to the transformed commits instead of the original commits. And this is where mirror trickery comes into play. You see, git keeps its branches and tags namespaced, so that that your local branches and tags can point someplace different from those on different remotes. When git checkout --mirror, you are telling git to create local branches and tags to match those on origin. When you git push --mirror --all, you are telling git to overwrite all branches and tags on origin with the _transformed_ refs you created locally. Again, if any old refs exist, the oversized file stays around. Mirroring actually has nothing to do with deep- vs shallow-clone, which is also a thing, but dealing with shallow clones is really complicated so everything in git uses deep clones by default.

----------

 Unfortunately yes, it is a best practice for site maintainers that comes at a cost to users and usability. This is one area where git is objectively worse than other SCM's, and LFS attempts to compensate without changing the git protocol itself. I personally think that the git maintainers should merge LFS or something similar into git itself. Fossil, a less-well-known competitor to git, did this from the start. However, we've got to use the tools that other people use.
 
Usually I try to be pragmatic about these decisions and not chase fads. In this case, I believe that peoples' repositories only ever become more complicated. Are you never going to add another solenoid or dipole map? Are you never going to modify the data inside? If this is indeed a one-off, it makes sense to keep it the way it is.
 
Git struggles with delta-compressing binary files, so even small modifications can lead to large increases in repository size. And even if all of your individual files are < 100MB or whatever, git will start hitting performance bottlenecks when the overall size of the repository > 500MB.
 
If you establish a convention for storing binary files outside of git, there will be a lot less confusion and wasted effort in the long run. Otherwise, I suspect that every few months someone will either:
 
(1) attempt to upload a >100MB file, fail because of the git hook, give up and not put it in a central location at all, and people will end up emailing the file around to each other
 
(2) modify large-ish files leading to a rapid increase of the overall repository size, making everything slow, and then then the whole collaboration has to slowly discover the cause of the problem and have this exact conversation all over again
 
Obviously it is your judgement call as to whether the tradeoff is worth it, I just want to share my two cents. :)



