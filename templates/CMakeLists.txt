#=======================================
# Template Files
#=======================================
set (dtd_files
  glabels-3.0.dtd
  glabels-4.0.dtd
)

set (template_files
  HONGRUN_14cm.template
)

set (other_db_files
  categories.xml
  paper-sizes.xml
  vendors.xml
)

# Export variables to parent scope
set (template_files ${template_files} PARENT_SCOPE)
set (other_db_files ${other_db_files} PARENT_SCOPE)

#=======================================
# Install
#=======================================
install (FILES ${dtd_files}      DESTINATION share/glabels-qt/templates)
install (FILES ${template_files} DESTINATION share/glabels-qt/templates)
install (FILES ${other_db_files} DESTINATION share/glabels-qt/templates)
