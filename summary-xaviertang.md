# Xavier's Summary

## What I did

I focused on creating the `Task.hpp` file, which involved defining most of the parameters that each Task object should have. This also included writing methods to edit and parse user input, particularly for converting strings to usable and storable date time strings.
I was also responsible for cleaning up and refactoring the code, which also involved removing unused dependencies and writing up the compilation instructions.
I also collaborated with my group members to make `Util.hpp` more usable, which previously was in a very messy state.

## Process & Reflection

This was a really messy project development process. It was largely unstructured.
Key takeaways
 - No matter how "simple" a project seems, in-depth planning will always be useful in a group
 This one was the biggest one. I believe that partially due to the perceived complexity of the project being fairly low, our group didn't really bother planning out the project too much or flesh out the details whatsoever. We just assigned roles and assumed that was it. At minimum we really just needed a planning document.

 - Communication holds projects together
 There was a distinct lack of communication from my group members. This definitely could have been improved, and not just in the planning phase, it was mostly throughout the project that there was really inconsistent communication. I myself am not innocent of this - I waited until the end of the mid semester break to reach out and ask for progress, when I really should have done it throughout the mid-semester break. Regardless this is one of the most important things that needed improvement.

Key Learnings
 - Interaction between system commands and C++
This was used for making and writing files, mostly for the user to user when editing certain aspects of the task.

 - Utilising temp directories for temporary storage
When making files, there needed to be somewhere to edit it, and out of the way such that the user should not have access to said file unless they explicitly look for it.
Also, making sure to remove the file after writing and reading from it.
 
 - SQLite 3 interactions with C++
I needed some understanding of this when rewriting some of the code for reading and writing tasks to the sqlite3 database.

## Potential Future Steps

This has been defined in a fairly modular way. It should be very easy to add functionality and provide a front end (if in the future that should happen). 

## Final Reflection

Overall, even though there were a lot of issues with the process itself, we managed to make a working program. The process was rough and that resulted in a lot of cleanup and refactoring that needed to be done. It would definitely have gone smoother with a better plan but even then it wouldn't have saved too much time. Everyone just had to pull themselves together towards the end to get the work done and that is what ended up happening - definitely not a good work ethic, but we got it done.
