########################
Bitbucket and repo setup
########################

.. include:: md2rst.txt

*************
Prerequisites
*************

Set up your profile on Bitbucket with ssh keys. You can follow the instructions
on the W-13 Confluence page `Gitting started with W-13 Bitbucket`_.

***************************************
Clone cpp\_stub into a local repository
***************************************

1. Navigate to the `upstream cpp\_stub repo`_.

2. Copy the ssh url from the Bitbucket "Clone" button on the Bitbucket
   repository web page. It should look like the following:

.. code:: bash

   ssh://git@xcp-stash.lanl.gov:7999/mm/cpp_stub.git

3. Navigate to your preferred repository directory in a terminal or use the
   example commands below

.. code:: bash

   # Start an ssh session to sstelmo.lanl.gov
   $ ssh -X sstelmo.lanl.gov
   # Note the present working directory (pwd) is your home directory
   $ pwd
   /home/<moniker>
   # OPTIONAL: Create a project space repository directory
   $ mkdir -p /projects/$USER/w13repos
   # Change pwd to repository directory
   $ cd /projects/$USER/w13repos
   # Double check pwd is repository directory
   $ pwd
   /projects/<moniker>/w13repos

4. Clone the stub repository using the url copied in step 2.

.. code:: bash

   # Double check pwd is repository directory
   $ pwd
   /projects/<moniker>/w13repos
   # Clone the stub repository
   $ git clone ssh://git@xcp-stash.lanl.gov:7999/mm/cpp_stub.git

5. Rename the local repository directory for your project.

.. code:: bash

   # Double check pwd is repository directory
   $ pwd
   /projects/<moniker>/w13repos
   # Observe the stub repo directory name
   $ ls cpp_stub -d
   cpp_stub
   # Rename the stub repo directory after your project
   $ mv cpp_stub my_project
   # Observe that the stub repo directory no longer exists
   $ ls cpp_stub -d
   ls: cannot access 'cpp_stub': No such file or directory
   # Observe that your project directory exists
   $ ls my_project -d
   my_project

6. Change to your project's repository directory

.. code:: bash

   # Double check pwd is repository directory
   $ pwd
   /projects/<moniker>/w13repos
   # Change to your project directory
   $ cd my_project
   # Double check pwd is your project directory
   $ pwd
   /projects/<moniker>/w13repos/my_project

************************************
Create a new repository on Bitbucket
************************************

.. note::

   These notes are a text copy of a variation on the `New Bitbucket Repo Guide`_
   which can also be found in the `W-13 DevOps Manual`_

1. Navigate to the W-13 `Material Models`_ Gitlab sub-group.

2. Create a new repository by clicking on the "+" sign in the upper left corner.

3. Enter a name for your project and click "Create repository"

4. Follow the "My code is already tracked by Git" instructions.

.. code:: bash

   $ pwd
   /projects/<moniker>/w13repos/my_project
   $ git remote set-url origin ssh://git@xcp-stash.lanl.gov:7999/mm/my_project.git
   $ git push -u origin --all
   $ git push origin --tags

5. Refresh the Bitbucket webpage and verify that the repository code was pushed
   correctly. You should see a list of source files and this Bitbucket parsed
   ``README.md`` displayed. You can also select the drop down branch menu to
   view a "master" and "dev" branch.

***********************************
Update settings for your repository
***********************************

Bitbucket repositories in the `Material Models`_ Gitlab sub-group inherit permissions and settings from that sub-group.
This included read permission for the ``w13bitbucket`` UNIX group (`W-13 Managed UNIX Groups`_). For most developers,
these inherited repository settings are appropriate and only a small number of settings must be updated.

1. Click on the gear icon in the lower left sidebar.

2. From the "Repository details" landing page, update the default branch from
   "master" to "dev".

3. From the "Repository permissions" tab you can add additional permissions by
   user and UNIX group.

4. From the "Default reviewers" tab you can add yourself and any project
   co-owners as default reviewers.

****************************
Fork the upstream repository
****************************

In the `Forking Workflow`_ the repository you just created in the `Material Models`_ Gitlab sub-group is called
the "upstream" repository. Throughout older W-13 documentation this may also be called the "official" repository.

Bitbucket repositories that inherit permissions from W-13 projects use the `Forking Workflow`_ and limit permissions for
pushing changes to the upstream repository. Now that branches exist on this repository, no one will be able to push
directly to *existing branches* of the upstream respository.

1. Click the fork button in the left hand sidebar just above the gear icon.

2. Click "fork this repository" button.

3. Verify

   a. The "Project" points to your personal project space in Bitbucket. It will
      probably show your full name as it appears in the phonebook.

   b. The "Name" matches your project name

   c. The "Enable fork syncing" checkbox is checked

4. Click the "Fork Repository" button. You should land on a familiar looking
   source code view of your repository, but now located in your personal project
   space.

This repository is referred to as the "fork" or "remote" repository throughout
W-13 DevOps documentation.

**********************************************
Update the remote url in your local repository
**********************************************

The final repo setup step is to update the remote url of the local clone of
``my_project``.  We will return to the terminal session.

1. Copy the url of your "remote" repository from the Bitbucket webpage. It
should look like:

.. code:: bash

   ssh://git@xcp-stash.lanl.gov:7999/~<moniker>/my_project.git

2. Return to your terminal session and update the remote repository for the
   final time.

.. code:: bash

   $ pwd
   /projects/<moniker>/w13repos/my_project
   $ git remote set-url origin ssh://git@xcp-stash.lanl.gov:7999/~<moniker>/my_project.git
   $ git push -u origin --all
   $ git push origin --tags

*****************************************
Update project name throughout repository
*****************************************

.. note::

   Note: the remaining steps are a truncated version of the W-13 Git project
   `ECMF contribution guide`_.  which can also be found in the `W-13 DevOps Manual`_.
   Critically, these steps will omit the Jira task creation and
   Bitbucket Pull-Request (PR) steps. The Bitbucket PR steps may be reproduced
   using the contribution guide, but your project will have to create a Jira
   project prior to integrating the Jira workflow. Contact the xcp devops team
   <devops-help@lanl.gov> to create a Jira project. You can email the W-13 DevOps
   team <w13devops@lanl.gov> for notes about setup.

1. Create a feature branch for your project name updates

.. code:: bash

   $ pwd
   /projects/<moniker>/w13repos/my_project
   $ git checkout -b feature/project-name-updates
   $ git branch
     dev
   * feature/project-name-updates
     master

2. Search for all instances of ``cpp_stub``. The list of occurrences will look
   quite long, but we can search and replace with ``sed`` to avoid manual file
   edits.

.. code:: bash

   $ pwd
   /projects/<moniker>/w13repos/my_project

   # Recursive, case-insensitive search and count occurrences
   $ grep -ri cpp_stub . --exclude-dir={build,.git} | wc -l
   57

   # Recursive, case-insensitive search and display
   $ grep -ri cpp_stub . --exclude-dir={build,.git}
   ...

   # Clean list of files with project name
   $ grep -ri cpp_stub . --exclude-dir={build,.git} -l
   ./CMakeLists.txt
   ./docs/api.rst
   ./docs/devops.rst
   ./README.md
   ./set_vars.sh
   ./src/cpp/cpp_stub.cpp
   ./src/cpp/cpp_stub.h
   ./src/cpp/tests/test_cpp_stub.cpp

3. Search and replace from command line

.. code:: bash

   $ pwd
   /projects/<moniker>/w13repos/my_project

   # Replace lower case occurrences in place
   $ sed -i 's/cpp_stub/my_project/g' $(grep -ri cpp_stub . --exclude-dir={build,.git} -l)
   $ grep -ri cpp_stub . --exclude-dir={build,.git} -l
   ./src/cpp/cpp_stub.h

   # Replace upper case occurrences in place
   $ sed -i 's/CPP_STUB/MY_PROJECT/g' $(grep -ri cpp_stub . --exclude-dir={build,.git} -l)

4. Verify no more occurrences of project name ``cpp_stub``

.. code:: bash

   $ pwd
   /projects/<moniker>/w13repos/my_project
   $ grep -ri cpp_stub . --exclude-dir={build,.git} | wc -l
   0
   $ grep -ri cpp_stub . --exclude-dir={build,.git}
   # no stdout to terminal because no files found
   $ grep -ri cpp_stub . --exclude-dir={build,.git} -l
   # no stdout to terminal because no files found

5. Search and replace camelcase project name occurrences, e.g. ``cppStub``.

.. code:: bash

   $ grep -r cppStub . --exclude-dir={build,.git}
   ...
   $ sed -i 's/cppStub/myProject/g' $(grep -r cppStub . --exclude-dir={build,.git} -l)
   $ grep -r cppStub . --exclude-dir={build,.git} -l
   # no stdout to terminal because no files found

6. Find files containing the project in their file name

.. code:: bash

   $ pwd
   /projects/<moniker>/w13repos/my_project
   $ find . -type d \( -name .git -o -name build \) -prune -false -o -name "*cpp_stub*"
   ./src/cpp/cpp_stub.cpp
   ./src/cpp/cpp_stub.h
   ./src/cpp/tests/test_cpp_stub.cpp 

7. Rename files after current project

.. code:: bash

   $ rename 's/cpp_stub/myproject/' $(find . -type d \( -name .git -o -name build \) -prune -false -o -name "*cpp_stub*")

8. Commit and push your changes to your "remote" or "fork" repository

.. code:: bash

   $ pwd
   /projects/<moniker>/w13repos/my_project
   # Add tracked files and message
   $ git commit -a -m "FEAT: replace cpp_stub with my_project through repository"
   $ git push origin feature/project-name-updates

You can also perform some cleanup in ``README.md`` to remove this walk-through.

From here, the W-13 best practice workflow would return to the Bitbucket webpage
and submit a Pull-Request from the ``feature/project-name-updates`` branch of
"\<moniker\>/my_project" repository (a.k.a. fork or remote) to the ``dev`` branch
of "Material Models/my_project" repository (a.k.a. upstream or official).

After updating your project by merging to the upstream repository, the fork
syncing feature of Bitbucket will automatically update any identically named
branches in your fork repository. Best practices suggest you should limit the
upstream repository branches to clean ``dev`` and ``master`` branches and
*NEVER* develop directly on the ``dev`` and ``master`` branches of your fork
repository. Limit development work to ``feature/thing`` type branches on your
fork/remote repo and frequently commit changes and push from the local feature
branch back to the fork/remote repo.

Happy hacking!
