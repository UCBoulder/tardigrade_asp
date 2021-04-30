# Retrieve and/or backup a Jenkins job

It may be possible to run the Jenkins backup manager as a non-admin, but
retrieving the xml file for a job requires access to the institutional W-13 user
account ``toolbox-jenkins``. Access is reserved for the W-13 DevOps team.

To get a copy of a Jenkins job xml file for revision control or backup, contact
a member of the W-13 DevOps team. What follows is the manual used by the DevOps
team.

1. From the [Jenkins landing/main page](https://toolbox-jenkins.lanl.gov/)
   select "Manage Jenkins" in the left sidebar.

2. Scroll all the way down to select "Backup Manager" in the "Uncategorized"
   section.

3. You can double check the settings

   * Backup configuration

     * Hudson root directory ``/local/jenkins-homes/toolbox-jenkins``
     * Backup directory: ``/home/toolbox-jenkins/backups``
     * Format: ``tar.bz2``
     * File name template: ``bakup_@date@.@extension@``
     * Custom exclusions: ``archive``
     * Verbose mode: checked
     * Configuration files (.xml) only: checked

       * Note: when this option is unchecked, the Jenkins plugins, general
         server configuration, and home space is also backed up. This includes
         ssh keys for the instutional account which should not be shared.

     * No shutdown: checked

   * Backup content

     * Backup job workspace: unchecked **NOTE: checking this box will vastly
       increase the backup file's disk space requirements! Use with caution.**
     * Backup builds history: checked **Note: these are relatively small build
       meta data files about success/failure.**
     * Backup maven artifacts archives: checked **Note: these file sizes depend
       entirely on what a project owner decides to archive to ninetails after
       the build. This option could be large if someone decided to copy anything
       other than pytest results xml files or small text files. If everyone is
       following good practices, these should be small.**
     * Backup fingerprints: checked

4. Press the "Backup Hudson Configuration" and wait

   * If you uncheck "backup configuration file only" the backup can take nearly
     an hour because it will backup all of the Jenkins configuration and plugin
     files in addition to job configuration files. With that option unchecked, the
     backup often requires 40 minutes.
   * If you click away from the "Backup manager log" webpage, you won't be able
     to get back to it. You'll be waiting blind for the job to finish. You can
     check progress by watching the backup file build and change size at

         toolbox-jenkins@ninetails:~/backups$ pwd
         /home/toolbox-jenkins/backups
         toolbox-jenkins@ninetails:~/backups$ ll -h
         total 1.3G
         -rw-r--r--. 1 toolbox-jenkins toolbox-jenkins 278M Apr  7  2020
         bakup_20200407_0939.tar.bz2
         -rw-r--r--. 1 toolbox-jenkins toolbox-jenkins 284M Apr 30 13:37
         bakup_20200430_1312.tar.bz2
         -rw-r--r--. 1 toolbox-jenkins toolbox-jenkins 284M Jun  8 14:16
         bakup_20200608_1352.tar.bz2
         -rw-r--r--. 1 toolbox-jenkins toolbox-jenkins 289M Jun 22 15:20
         bakup_20200622_1453.tar.bz2
         -rw-r--r--. 1 toolbox-jenkins toolbox-jenkins 183M Oct 22 11:32
         bakup_20201022_1119.tar.bz2

     with a command like

         toolbox-jenkins@ninetails:~/backups$ watch ls -lh bakup_20201022_1119.tar.bz2

     When it stops changing size for several minutes, the backup is probably
     complete.

5. Copy the tar backup file off of ninetails. For example:

       toolbox-jenkins@ninetails:~/backups$ scp bakup_20201022_1119.tar.bz2 roppenheimer@sstelmo.lanl.gov:/projects/roppenheimer

6. Remove permissions from group and other

       [roppenheimer@sstelmo roppenheimer]$ chmod go-r bakup_20201022_1119.tar.bz2

7. Extract the tarball to a specific directory

       [roppenheimer@sstelmo roppenheimer]$ mkdir bakup_20201022_1119/
       [roppenheimer@sstelmo roppenheimer]$ chmod go-rx bakup_20201022_1119
       [roppenheimer@sstelmo roppenheimer]$ tar -xf bakup_20201022_1119.tar.bz2 --directory bakup_20201022_1119/

8. Find the job that you want to backup/version control/examine/copy.

       [roppenheimer@sstelmo roppenheimer]$ find bakup_20201022_1119/ -type f -name config.xml | grep cpp_stub
       bakup_20201022_1119/jobs/cpp_stub_Pull_Request_Builder/config.xml
