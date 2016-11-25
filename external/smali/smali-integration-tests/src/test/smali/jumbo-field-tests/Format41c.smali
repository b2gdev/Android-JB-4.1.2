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

.class public LFormat41c;
.super Ljava/lang/Object;
.source "Format41c.smali"

.method public constructor <init>()V
    .registers 1
    invoke-direct {p0}, Ljava/lang/Object;-><init>()V
    return-void
.end method

.method public test-sput-sget-jumbo()V
    .registers 258
    .annotation runtime Lorg/junit/Test;
    .end annotation

    const v0, 23
    move/16 v256, v0
    sput/jumbo v256, LManyStaticFields;->field99999:I

    sget/jumbo v257, LManyStaticFields;->field99999:I

    invoke-static/range {v256 .. v257}, LAssert;->assertEquals(II)V
    return-void
.end method

.method public test-sput-object-sget-object-jumbo()V
    .registers 258
    .annotation runtime Lorg/junit/Test;
    .end annotation

    new-instance v0, Ljava/lang/Object;
    invoke-direct {v0}, Ljava/lang/Object;-><init>()V

    move-object/16 v256, v0

    sput-object/jumbo v256, LManyStaticFields;->field99999Object:Ljava/lang/Object;

    sget-object/jumbo v257, LManyStaticFields;->field99999Object:Ljava/lang/Object;

    invoke-static/range {v256 .. v257}, Lorg/junit/Assert;->assertEquals(Ljava/lang/Object;Ljava/lang/Object;)V
    return-void
.end method

.method public test-sput-wide-sget-wide-jumbo()V
    .registers 260
    .annotation runtime Lorg/junit/Test;
    .end annotation

    const-wide v0, 0x200000000L
    move-wide/16 v256, v0

    sput-wide/jumbo v256, LManyStaticFields;->field99999Wide:J

    sget-wide/jumbo v258, LManyStaticFields;->field99999Wide:J

    invoke-static/range {v256 .. v259}, Lorg/junit/Assert;->assertEquals(JJ)V
    return-void
.end method

.method public test-sput-boolean-sget-boolean-true-jumbo()V
    .registers 258
    .annotation runtime Lorg/junit/Test;
    .end annotation

    const v0, 1
    move/16 v256, v0

    sput-boolean/jumbo v256, LManyStaticFields;->field99999Boolean:Z

    sget-boolean/jumbo v257, LManyStaticFields;->field99999Boolean:Z

    invoke-static/range {v257}, Lorg/junit/Assert;->assertTrue(Z)V
    return-void
.end method

.method public test-sput-boolean-sget-boolean-false-jumbo()V
    .registers 258
    .annotation runtime Lorg/junit/Test;
    .end annotation

    const v0, 0
    move/16 v256, v0

    sput-boolean/jumbo v256, LManyStaticFields;->field99999Boolean:Z

    sget-boolean/jumbo v257, LManyStaticFields;->field99999Boolean:Z

    invoke-static/range {v257}, Lorg/junit/Assert;->assertFalse(Z)V
    return-void
.end method

.method public test-sput-byte-sget-byte-jumbo()V
    .registers 258
    .annotation runtime Lorg/junit/Test;
    .end annotation

    const v0, 120T
    move/16 v256, v0

    sput-byte/jumbo v256, LManyStaticFields;->field99999Byte:B

    sget-byte/jumbo v257, LManyStaticFields;->field99999Byte:B

    invoke-static/range {v256 .. v257}, LAssert;->assertEquals(II)V
    return-void
.end method

.method public test-sput-char-sget-char-jumbo()V
    .registers 258
    .annotation runtime Lorg/junit/Test;
    .end annotation

    const v0, 'a'
    move/16 v256, v0

    sput-char/jumbo v256, LManyStaticFields;->field99999Char:C

    sget-char/jumbo v257, LManyStaticFields;->field99999Char:C

    invoke-static/range {v256 .. v257}, LAssert;->assertEquals(II)V
    return-void
.end method

.method public test-sput-short-sget-short-jumbo()V
    .registers 258
    .annotation runtime Lorg/junit/Test;
    .end annotation

    const v0, 1234S
    move/16 v256, v0

    sput-short/jumbo v256, LManyStaticFields;->field99999Short:S

    sget-short/jumbo v257, LManyStaticFields;->field99999Short:S

    invoke-static/range {v256 .. v257}, LAssert;->assertEquals(II)V
    return-void
.end method