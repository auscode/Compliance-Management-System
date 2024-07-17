#include "bitmap.h"

/*
This function adds a compliance incident to a compliance management system.
It checks if the system is not already full, if the incident type is valid,
if the incident description is valid, and if the incident severity is valid.
If all checks pass, the function adds the incident to the system and increments
the number of incidents in the system. If any of the checks fail, the function
returns without adding the incident to the system.
*/
void addComplianceIncident(ComplianceManagementSystem *system, ComplianceIncident incident)
{
    // Check if the system is full
    if (system->numIncidents == 100)
    {
        return;
    }

    // Check if the incident type is valid
    if (incident.type != DATA_PRIVACY && incident.type != FINANCIAL_REGULATIONS &&
        incident.type != EMPLOYMENT_LAWS && incident.type != ENVIRONMENTAL_REGULATIONS)
    {
        return;
    }

    // Check if the incident description is valid
    if (strlen(incident.description) == 0 || strlen(incident.description) > 100)
    {
        return;
    }

    // Check if the incident severity is valid
    if (incident.severity < 1 || incident.severity > 10)
    {
        return;
    }

    // Add the incident to the system
    switch (incident.type)
    {
    case DATA_PRIVACY:
        system->incidents[system->numIncidents].dataPrivacyIncident = incident;
        break;
    case FINANCIAL_REGULATIONS:
        system->incidents[system->numIncidents].financialRegulationsIncident = incident;
        break;
    case EMPLOYMENT_LAWS:
        system->incidents[system->numIncidents].employmentLawsIncident = incident;
        break;
    case ENVIRONMENTAL_REGULATIONS:
        system->incidents[system->numIncidents].environmentalRegulationsIncident = incident;
        break;
    }

    // Increment the number of incidents in the system
    system->numIncidents++;
}

/*
The function takes as input a ComplianceManagementSystem object and calculates the
average severity of all incidents in the system. It first checks if there are any
incidents in the system and returns 0 if there are none. Then, it loops through
all incidents in the system and adds up the severity of each one based on its type.
Finally, it calculates the average severity and returns it as a float value.
*/
float calculateAverageSeverity(ComplianceManagementSystem system)
{
    // Check if there are no incidents in the system
    if (system.numIncidents == 0)
    {
        return 0.0;
    }

    // Calculate the total severity of all incidents in the system
    int totalSeverity = 0;
    for (int i = 0; i < system.numIncidents; i++)
    {
        ComplianceIncident incident = system.incidents[i].dataPrivacyIncident; // initialize to first incident in union
        switch (system.incidents[i].dataPrivacyIncident.type)
        {
        case DATA_PRIVACY:
            incident = system.incidents[i].dataPrivacyIncident;
            break;
        case FINANCIAL_REGULATIONS:
            incident = system.incidents[i].financialRegulationsIncident;
            break;
        case EMPLOYMENT_LAWS:
            incident = system.incidents[i].employmentLawsIncident;
            break;
        case ENVIRONMENTAL_REGULATIONS:
            incident = system.incidents[i].environmentalRegulationsIncident;
            break;
        default:
            break; // handle error case
        }
        totalSeverity += incident.severity;
    }

    // Calculate the average severity
    float averageSeverity = (float)totalSeverity / system.numIncidents;

    return averageSeverity;
}

/*
This function removes all incidents of a given type from a Compliance Management System.
It loops through all incidents in the system, checks if the incident matches the type to be removed,
shifts all incidents after this one back by one position, decrements the number of incidents in the system,
and increments the number of removed incidents. Finally, it zeros out the remaining incidents that were
shifted back and returns the number of incidents that were removed.
*/
int removeComplianceIncidentsOfType(ComplianceManagementSystem *system, ComplianceType type)
{
    int numRemoved = 0;
    // Check if the system is empty
    if (system->numIncidents == 0)
    {
        return numRemoved;
    }
    // Loop through all incidents in the system
    for (int i = 0; i < system->numIncidents; i++)
    {
        ComplianceIncident incident = system->incidents[i].dataPrivacyIncident;
        // Check if the incident matches the type to be removed
        if (incident.type == type)
        {
            // Shift all incidents after this one back by one position
            for (int j = i; j < system->numIncidents - 1; j++)
            {
                system->incidents[j] = system->incidents[j + 1];
            }
            // Decrement the number of incidents in the system
            system->numIncidents--;

            // Increment the number of removed incidents
            numRemoved++;
        }
    }
    // Zero out the remaining incidents that were shifted back
    for (int i = system->numIncidents; i < 100; i++)
    {
        memset(&system->incidents[i], 0, sizeof(ComplianceIncidentUnion));
    }
    return numRemoved;
}

/*
This function takes a ComplianceManagementSystem object as input, which contains a list of incidents,
and returns the incident with the highest severity. If there are no incidents, the function returns a
default empty incident with a data privacy type and a message stating "No incidents in the system."
The function loops through all incidents in the system and compares their severity with the current highest severity incident,
updating it if a higher severity incident is found. The function returns the highest severity incident found.

*/
ComplianceIncident findHighestSeverityIncident(ComplianceManagementSystem system)
{
    // Check if there are any incidents in the system
    if (system.numIncidents == 0)
    {
        ComplianceIncident emptyIncident = {DATA_PRIVACY, "No incidents in the system", 0};
        return emptyIncident;
    }
    // Initialize highest severity incident to the first incident in the system
    ComplianceIncident highestSeverityIncident = system.incidents[0].dataPrivacyIncident;
    // Loop through all incidents in the system and find the one with the highest severity
    for (int i = 0; i < system.numIncidents; i++)
    {
        ComplianceIncident currentIncident;
        switch (system.incidents[i].dataPrivacyIncident.type)
        {
        case DATA_PRIVACY:
            currentIncident = system.incidents[i].dataPrivacyIncident;
            break;
        case FINANCIAL_REGULATIONS:
            currentIncident = system.incidents[i].financialRegulationsIncident;
            break;
        case EMPLOYMENT_LAWS:
            currentIncident = system.incidents[i].employmentLawsIncident;
            break;
        case ENVIRONMENTAL_REGULATIONS:
            currentIncident = system.incidents[i].environmentalRegulationsIncident;
            break;
        }
        // If the current incident has a higher severity than the highestSeverityIncident, update highestSeverityIncident
        if (currentIncident.severity > highestSeverityIncident.severity)
        {
            highestSeverityIncident = currentIncident;
        }
    }
    return highestSeverityIncident;
}

/*
This function updates the severity of a compliance incident in a management system.
It takes three parameters: a pointer to the compliance management system, the compliance incident to be updated,
and the new severity value. The function searches for the incident in the system, and if found, checks if the
new severity value is within the allowed range of 1-10. If the new severity is within the range, it updates
the incident's severity and returns 0 to indicate success. If the new severity is out of range, it returns 1.
If the incident is not found in the system, it returns -1.
*/
int updateComplianceIncidentSeverity(ComplianceManagementSystem *system, ComplianceIncident incident, int newSeverity)
{
    int found = 0; // flag to keep track if the incident was found
    int i;
    for (i = 0; i < system->numIncidents; i++)
    {
        // check if the incident type and description match
        if (system->incidents[i].dataPrivacyIncident.type == incident.type &&
            strcmp(system->incidents[i].dataPrivacyIncident.description, incident.description) == 0)
        {
            found = 1; // set the found flag
            // check if the new severity is within the allowed range of 1-10
            if (newSeverity < 1 || newSeverity > 10)
            {
                return 1; // return 1 to indicate the new severity is out of range
            }
            else
            {
                // update the severity of the incident
                system->incidents[i].dataPrivacyIncident.severity = newSeverity;
                return 0; // return 0 to indicate the incident was successfully updated
            }
        }
    }
    if (!found)
    {
        return -1; // return -1 to indicate the incident was not found in the system
    }
    return 0;
}

/*
This function removes a compliance incident from a management system by finding the index of the
incident to be removed, shifting all incidents after the removed incident back by one index,
and decrementing the number of incidents in the system. If the incident is not found,
the function returns without removing anything.
*/
void removeComplianceIncident(ComplianceManagementSystem *system, ComplianceIncident incident)
{
    int incidentIndex = -1;
    // Find the index of the incident to be removed
    for (int i = 0; i < system->numIncidents; i++)
    {
        if (memcmp(&system->incidents[i], &incident, sizeof(ComplianceIncident)) == 0)
        {
            incidentIndex = i;
            break;
        }
    }
    // If the incident was not found, return without removing anything
    if (incidentIndex == -1)
    {
        return;
    }
    // Shift all incidents after the removed incident back by one index
    for (int i = incidentIndex; i < system->numIncidents - 1; i++)
    {
        system->incidents[i] = system->incidents[i + 1];
    }
    // Decrement the number of incidents in the system
    system->numIncidents--;
}
