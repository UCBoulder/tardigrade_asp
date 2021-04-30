# Create a new Jenkins job from template

From the main page: https://toolbox-jenkins.lanl.gov/

    1. Select "New Item" in the left hand sidebar.
    2. Give the project a name following the current convention: "<project name>_Pull_Request_Builder" or "cpp_stub_Pull_Request_Builder"
    3. We want to copy another job as a template file. Enter "cpp_stub_Pull_Request_Builder" in the "Copy from" text field.

       * NOTE: the job configuration can also be found as ``config.xml`` in this
         repository.

    4. Click ok
    5. Update the project information from the template job

       1. "General: Description" can stay, or you can add project details
       2. "General: This project is parameterized" provides quasi-protection
          from people building your job manually. They can still build it manually, but
          they have to consciously choose to check a box that says "Prevent manual
          building" before they can manually build your job...
       3. "General: Restrict where this project can be run" is good with "sstelmo"
          for now. You can update as W-13 builds support for more servers, e.g. HPC snow.
       4. "Source Code Management: Git" make sure the Git radial button is
          selected.

          1. The repository url should be updated to match your repo. If you're
             doing things the W-13 way, this would fit the convention
             "ssh://git@xcp-stash.lanl.gov:7999/mm/<project_name>.git" or here
             "ssh://git@xcp-stash.lanl.gov:7999/mm/cpp_stub.git"
          2. Leave everything else alone. If you have to change anything else in
             the "Source Code Management" section, then you are way out of scope for a stub
             repo tutorial or need to seek help from a project owner for out-of-W-13 setup
             guides/templates.
       5. "Build Triggers: Stash pull request builder"

          1. Update "project" with the project key of the Bitbucket project, e.g.
             the material models key is MM. You can find the key on the list of Bitbucket
             projects, https://xcp-stash.lanl.gov/projects, or in the project settings if you
             have access. For W-13 subroutine projects, you should be using the Material
             Model project and won't need to change this.
           2. Update "repository name" to your projects name. This is case
              insensitive. In this case, "cpp_stub".
           3. You can make changes to the cron schedule if you like, this project
              pings the Bitbucket server every minute. No xcp-devops admin has complained
              yet...
           4. DO NOT change the "stash URL" or "stash credentials" for W-13
              repositories.

       6. "Build Environment: Delete workspace before build starts" is checked to
          avoid false positive/negatives caused by leftover files. If you want to save
          build time and avoid re-compiling objects when there is no change in source, you
          can uncheck this box.
       7. "Build: Execute shell" is more or less a bash script. You can put any
          bash commands here, but realize that they won't be version controlled. W-13
          projects have settled on the standard of only executing a "jenkins_build.sh"
          script so all of the CI build commands are version controlled in the repository.
          If you want to integrate your repository with the W-13 Python Environment
          integration tests you MUST have a "jenkins_build.sh" script as the SOLE source
          of your integration test commands.

          * https://simulia.lanl.gov/ECMF-D/devops_guide.html#continuous-integration
          * https://xcp-confluence.lanl.gov/display/COM/W-13+DevOps

       8. "Post-build Actions" is completely project specific.

          1. Most W-13 PR build scripts copy any test results files to the
             toolbox-jenkins server for archival. This aids in debugging when you follow a PR
             build link to the Jenkins report and is especially important if you want to view
             results of old jobs but you have "delete workspace before build starts" checked.
          2. Some W-13 CI jobs also have email alerts for failed builds. This is
             not so important for a PR builder because the job results will be posted back to
             the Bitbucket Pull-Request as a comment.
