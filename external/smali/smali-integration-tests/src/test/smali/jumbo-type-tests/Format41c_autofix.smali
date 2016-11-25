#Copyright 2011, Google Inc.
#All rights reserved.
#
#Redistribution and use in source and binary forms, with or without
#modification, are permitted provided that the following conditions are
#met:
#
#    * Redistributions of source code must retain the above copyright
#notice, this list of conditions and the following disclaimer.
#    * Redistributions in binary form must reproduce the above
#copyright notice, this list of conditions and the following disclaimer
#in the documentation and/or other materials provided with the
#distribution.
#    * Neither the name of Google Inc. nor the names of its
#contributors may be used to endorse or promote products derived from
#this software without specific prior written permission.
#
#THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
#"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
#LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
#A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
#OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
#SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
#LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
#DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
#THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
#(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
#OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

.class public LFormat41c_autofix;
.super Ljava/lang/Object;
.source "Format41c_autofix.smali"

.method public constructor <init>()V
    .registers 1
    invoke-direct {p0}, Ljava/lang/Object;-><init>()V
    return-void
.end method

.method public test_const-class-jumbo()V
    .registers 9
    .annotation runtime Lorg/junit/Test;
    .end annotation

    const-class v0, Lzzz99999;
    invoke-virtual {v0}, Ljava/lang/Class;->getName()Ljava/lang/String;
    move-result-object v0

    #check for the initial 3 z's
    const v1, 3
    const v2, 0
    const-wide v3, 'z'

    :loop
        invoke-virtual {v0, v2}, Ljava/lang/String;->charAt(I)C
        move-result v5

        int-to-long v6, v5

        invoke-static {v3, v4, v6, v7}, Lorg/junit/Assert;->assertEquals(JJ)V

	add-int/lit8 v2, v2, 1
    if-ne v1, v2, :loop

    #and now for the final 9's
    invoke-virtual {v0}, Ljava/lang/String;->length()I
    move-result v1
    const-wide v3, '9'

    :loop2
        invoke-virtual {v0, v2}, Ljava/lang/String;->charAt(I)C
        move-result v5

        int-to-long v6, v5

        invoke-static {v3, v4, v6, v7}, Lorg/junit/Assert;->assertEquals(JJ)V

	add-int/lit8 v2, v2, 1
    if-ne v1, v2, :loop2

    return-void
.end method

.method public test_new-instance-jumbo-index()V
    .registers 3
    .annotation runtime Lorg/junit/Test;
    .end annotation

    new-instance v1, Lzzz99999;

    #there's no way to initialize the new object, due to type index constraints on a method_id_item
    #so just attempt to create the new object and call it good

    return-void
.end method

.method public test_check-cast-jumbo-fail()V
    .registers 2
    .annotation runtime Lorg/junit/Test;
    .end annotation

    :tryStart
    const-string v0, "test"

    check-cast v0, Lzzz99999;
    :tryEnd
    .catch Ljava/lang/ClassCastException; {:tryStart .. :tryEnd} :handler

    #the check-cast didn't throw an exception as expected
    invoke-static {}, Lorg/junit/Assert;->fail()V

    :handler

    return-void
.end method
