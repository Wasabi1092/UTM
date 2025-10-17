# Jia Ying's Summary

## What I did

My main contribution focused on implementing the configuration system (`Config.hpp`) that manages subjects and their associated colours using SQLite. I wrote the logic to add new subjects, check for duplicates, edit existing subject colours, and retrieve subject colours for use elsewhere in the program. This included proper input validation and careful handling of SQLite prepared statements to prevent data corruption or overwriting.

At the start of the project, I worked on implementing the list-task feature, which was originally designed to manage and display tasks stored in a vector<Task>. I focused on writing helper functions to list all existing tasks, as well as adding getter and setter methods in `Task.hpp` (such as for status and priority) to support task sorting and filtering.

I mainly worked independently on these parts, but I also collaborated with teammates to ensure that my work integrated correctly with their parts - particularly the task management and database initialisation.

## Process & Reflection

The development process for this project was quite unstructured and sometimes disorganised. Our group initially didn't have a clear plan or communication routine, which made it difficult to keep track of each person's progress and how all the parts would fit together. This caused overlapping work and confusion at times, especially when trying to merge our changes.

However, this experience taught me the importance of consistent communication, clear task division, and writing an early planning document. Even though we faced coordination and communication challenges, it was rewarding to see the project come together at the end.

### Key Takeaways
- Planning is critical. We underestimated how much structure was needed. Defining a proper plan and timeline would have saved a lot of back-and-forth later.
- Communication keeps everyone aligned. There were gaps in communication that made collaboration harder. Regular check-ins or updates would have improved efficiency and teamwork.
- Technical learning:
    - Gained practical experience with SQLite and prepared statements in C++.
    - Learned how to bind text parameters safely and query data efficiently.
    - Practised modular programming by separating configuration logic from other system functionalities.
    - Strengthened my debugging and error-handling skills, particularly when dealing with database queries.

## Potential Future Steps

The configuration system could be expanded to allow users to manage more settings, such as theme or default views. The task manager could also be extended with colour-coded task displays based on subjects or priority, improving usability.

## Final Reflection

Although the project had a rocky start due to lack of planning and communication, it was still a meaningful learning experience. I learned not just about technical implementation, but also about the importance of teamwork and coordination in software development.

Overall, this project taught me not only about C++ programming in software development and database management using SQLite, but also about the value of teamwork, communication, and planning in software projects. Going forward, I'll apply what I learned about structured planning, clearer communication, and modular design to ensure smoother collaboration in future projects.